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

#include "ExemplarTypeLogger.h"
#include "ExemplarTypes.h"
#include "Logger.h"
#include "cGZPersistResourceKey.h"
#include "cIGZPersistResource.h"
#include "cIGZVariant.h"
#include "cISCProperty.h"
#include "cISCPropertyHolder.h"
#include "cRZAutoRefCount.h"
#include "SCPropertyUtil.h"

static constexpr uint32_t ExemplarTypePropertyID = 0x00000010;

ExemplarTypeLogger::ExemplarTypeLogger(bool logResourceLoadErrors)
	: ExemplarResourceLoggerBase(logResourceLoadErrors)
{
}

void ExemplarTypeLogger::ProcessExemplarResource(
	const char* const originalFunctionName,
	cIGZPersistResource* pResource)
{
	Logger& logger = Logger::GetInstance();

	uint32_t exemplarType = 0;

	cRZAutoRefCount<cISCPropertyHolder> propertyHolder;
	bool wroteExemplarType = false;

	if (pResource->QueryInterface(GZIID_cISCPropertyHolder, propertyHolder.AsPPVoid()))
	{
		if (SCPropertyUtil::GetPropertyValue(propertyHolder, ExemplarTypePropertyID, exemplarType))
		{
			cGZPersistResourceKey key;
			pResource->GetKey(key);

			if (logger.IsEnabled(LogLevel::Debug))
			{
				logger.WriteLineFormatted(
					LogLevel::Debug,
					"%s: T=0x%08X G=0x%08X, I=0x%08X, ExemplarType=%s",
					originalFunctionName,
					key.type,
					key.group,
					key.instance,
					ExemplarTypes::GetExemplarTypeName(exemplarType));
			}
			else
			{
				logger.WriteLineFormatted(
					LogLevel::Info,
					"T=0x%08X G=0x%08X, I=0x%08X, ExemplarType=%s",
					key.type,
					key.group,
					key.instance,
					ExemplarTypes::GetExemplarTypeName(exemplarType));
			}

			wroteExemplarType = true;
		}
	}

	if (!wroteExemplarType)
	{
		LogExemplarTGI(originalFunctionName, pResource);
	}
}
