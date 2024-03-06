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
#include "ExemplarLoggerFactory.h"
#include "ExemplarErrorLogger.h"
#include "ExemplarTGILogger.h"
#include "ExemplarTypeLogger.h"
#include "ExemplarTypes.h"
#include "FilteredExemplarLogger.h"
#include "StringViewUtil.h"
#include "cIGZApp.h"
#include "cIGZCmdLine.h"
#include "cIGZCOM.h"
#include "cIGZFrameWork.h"
#include "cIGZPersistResourceManager.h"
#include "cRZAutoRefCount.h"
#include "cRZBaseString.h"
#include "cRZCOMDllDirector.h"
#include "cISC4App.h"
#include <GZServPtrs.h>

#include <algorithm>
#include <cctype>
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

using namespace std::string_view_literals;

static constexpr std::string_view PluginLogFileName = "SC4ExemplarLoad.log"sv;

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

	void SetLoggerFromCommandLine(const std::string_view& argName)
	{
		std::shared_ptr<IExemplarLogger> exemplarLogger;

		if (StringViewUtil::EqualsIgnoreCase(argName, "error"sv))
		{
			exemplarLogger = std::make_shared<ExemplarErrorLogger>();
		}
		else if (StringViewUtil::EqualsIgnoreCase(argName, "debug"sv))
		{
			Logger::GetInstance().SetLogLevel(LogLevel::Debug);
			exemplarLogger = std::make_shared<ExemplarTGILogger>(/*logResourceLoadErrors*/true);
		}
		else if (StringViewUtil::EqualsIgnoreCase(argName, "type"sv))
		{
			exemplarLogger = std::make_shared<ExemplarTypeLogger>();
		}
		else if (StringViewUtil::EqualsIgnoreCase(argName, "TGI"sv))
		{
			exemplarLogger = std::make_shared<ExemplarTGILogger>();
		}
		else
		{
			// Any other value is an integer that is used to filter for a specific
			// exemplar type.
			uint32_t type = 0;

			if (ExemplarTypes::TryParseExemplarNumber(argName, type))
			{
				exemplarLogger = std::make_shared<FilteredExemplarLogger>(type);
			}
			else
			{
				Logger::GetInstance().WriteLineFormatted(
					LogLevel::Error,
					"Invalid exemplar type argument '%s', expected an exemplar type number.",
					std::string(argName).c_str());
			}
		}

		if (exemplarLogger)
		{
			ExemplarLoggerFactory::SetInstance(std::move(exemplarLogger));
		}
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

		const cIGZCmdLine* pCmdLine = pFramework->CommandLine();

		cRZBaseString value;

		if (pCmdLine->IsSwitchPresent(cRZBaseString("exemplar-log"), value, true))
		{
			SetLoggerFromCommandLine(value.ToChar());
		}
		else
		{
			// We default to only logging errors.
			ExemplarLoggerFactory::SetInstance(std::move(std::make_shared<ExemplarErrorLogger>()));
		}

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

		if (!pResMan->RegisterObjectFactory(
			GZCLSID_ExemplarFactoryProxy,
			0x6534284A,
			nullptr))
		{
			Logger::GetInstance().WriteLine(
				LogLevel::Error,
				"Failed to register the exemplar factory proxy.");
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