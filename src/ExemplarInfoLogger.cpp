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

#include "ExemplarInfoLogger.h"
#include "Logger.h"
#include "cGZPersistResourceKey.h"
#include "cIGZPersistResource.h"
#include "cISCPropertyHolder.h"
#include "cRZAutoRefCount.h"

ExemplarInfoLogger& ExemplarInfoLogger::GetInstance()
{
	static ExemplarInfoLogger instance;

	return instance;
}

void ExemplarInfoLogger::SetPropertyWriter(ExemplarPropertyWriter* writer)
{
	propertyWriter = writer;
}

void ExemplarInfoLogger::WritePropertyData(
	const char* const originalFunctionName,
	uint32_t riid,
	void** ppvObj)
{
	Logger& logger = Logger::GetInstance();

	// SC4 appear to always set the requested interface ID for an exemplar resource
	// to cIGZPersistResource.
	if (riid == GZIID_cIGZPersistResource && *ppvObj)
	{
		cIGZPersistResource* pRes = static_cast<cIGZPersistResource*>(*ppvObj);
		cGZPersistResourceKey key;

		pRes->GetKey(key);

		cRZAutoRefCount<cISCPropertyHolder> propertyHolder;
		bool wroteLogEntry = false;

		// Try to convert the cIGZPersistResource into a cISCPropertyHolder.
		// If this succeeds we can query, add, and remove properties from the exemplar.
		if (pRes->QueryInterface(GZIID_cISCPropertyHolder, propertyHolder.AsPPVoid()))
		{
			if (propertyWriter)
			{
				wroteLogEntry = propertyWriter->WritePropertyData(
					originalFunctionName,
					key,
					propertyHolder);
			}
		}

		if (!wroteLogEntry)
		{
			if (logger.IsEnabled(LogLevel::Debug))
			{
				logger.WriteLineFormatted(
					LogLevel::Debug,
					"%s: T=0x%08X G=0x%08X, I=0x%08X, ExemplarType=%s",
					originalFunctionName,
					key.type,
					key.group,
					key.instance);
			}
			else
			{
				logger.WriteLineFormatted(
					LogLevel::Info,
					"T=0x%08X G=0x%08X, I=0x%08X",
					key.type,
					key.group,
					key.instance);
			}
		}
	}
	else
	{
		logger.WriteLineFormatted(
			LogLevel::Info,
			"%s: riid=0x%08X",
			originalFunctionName,
			riid);
	}
}

ExemplarInfoLogger::ExemplarInfoLogger()
	: propertyWriter(nullptr)
{
}
