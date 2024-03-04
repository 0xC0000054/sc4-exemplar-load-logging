///////////////////////////////////////////////////////////////////////////////
//
// This file is part of sc4-exemplar-load-logging, a DLL Plugin for SimCity 4
// that adds logging to the game's exemplar loading process.
//
// Copyright (c) 2024 Nicholas Hayes
//
// This file is licensed under terms of the MIT License.
// See LICENSE.txt for more information.
//
///////////////////////////////////////////////////////////////////////////////

#include "version.h"
#include "Logger.h"
#include "ExemplarFactoryProxy.h"
#include "ExemplarInfoLogger.h"
#include "ExemplarTypePropertyWriter.h"
#include "cIGZApp.h"
#include "cIGZCOM.h"
#include "cIGZFrameWork.h"
#include "cIGZPersistResourceManager.h"
#include "cRZAutoRefCount.h"
#include "cRZBaseString.h"
#include "cRZCOMDllDirector.h"
#include "cISC4App.h"
#include <GZServPtrs.h>

#include <filesystem>
#include <functional>
#include <memory>
#include <string>
#include <Windows.h>
#include "wil/resource.h"
#include "wil/win32_helpers.h"

#include "EASTLConfigSC4.h"
#include "EASTL\vector.h"

static constexpr uint32_t kExemplarLoadLoggingDirectorID = 0x1DFE8A62;

static constexpr std::string_view PluginLogFileName = "SC4ExemplarLoad.log";

namespace
{
	void PrintLineToDebugOutput(const char* const line)
	{
		OutputDebugStringA(line);
		OutputDebugStringA("\n");
	}

	cIGZUnknown* CreateExemplarFactoryProxy()
	{
		return new ExemplarFactoryProxy();
	}
}

class ExemplarLoadLoggingDllDirector : public cRZCOMDllDirector
{
public:

	ExemplarLoadLoggingDllDirector()
	{
		AddCls(GZCLSID_ExemplarFactoryProxy, CreateExemplarFactoryProxy);

		std::filesystem::path dllFolderPath = GetDllFolderPath();

		std::filesystem::path logFilePath = dllFolderPath;
		logFilePath /= PluginLogFileName;

		ExemplarInfoLogger::GetInstance().SetPropertyWriter(std::move(std::make_unique<ExemplarTypePropertyWriter>()));

		Logger& logger = Logger::GetInstance();
		logger.Init(logFilePath, LogLevel::Info, false);
		logger.WriteLogFileHeader("SC4ExemplarLoadLogging v" PLUGIN_VERSION_STR);
	}

	uint32_t GetDirectorID() const
	{
		return kExemplarLoadLoggingDirectorID;
	}

	bool OnStart(cIGZCOM* pCOM)
	{
		cIGZFrameWork* const pFramework = RZGetFrameWork();

		const cIGZFrameWork::FrameworkState state = pFramework->GetState();

		if (state < cIGZFrameWork::kStatePreAppInit)
		{
			pFramework->AddHook(this);
		}
		else
		{
			PreAppInit();
		}

		return true;
	}

	bool PreFrameWorkInit()
	{
		cIGZPersistResourceManagerPtr pResMan;

		if (pResMan->RegisterObjectFactory(
			GZCLSID_ExemplarFactoryProxy,
			0x6534284A,
			nullptr))
		{
			PrintLineToDebugOutput("Registered ExemplarFactoryProxy.");
		}

		return true;
	}

private:

	std::filesystem::path GetDllFolderPath()
	{
		wil::unique_cotaskmem_string modulePath = wil::GetModuleFileNameW(wil::GetModuleInstanceHandle());

		std::filesystem::path temp(modulePath.get());

		return temp.parent_path();
	}
};

cRZCOMDllDirector* RZGetCOMDllDirector() {
	static ExemplarLoadLoggingDllDirector sDirector;
	return &sDirector;
}