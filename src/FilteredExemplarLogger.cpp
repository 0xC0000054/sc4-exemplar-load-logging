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

#include "FilteredExemplarLogger.h"
#include "Logger.h"
#include "cGZPersistResourceKey.h"
#include "cIGZPersistResource.h"
#include "cIGZVariant.h"
#include "cISCProperty.h"
#include "cISCPropertyHolder.h"
#include "cRZAutoRefCount.h"
#include "SCPropertyUtil.h"

static constexpr uint32_t ExemplarTypePropertyID = 0x00000010;

FilteredExemplarLogger::FilteredExemplarLogger(uint32_t type)
	: ExemplarResourceLoggerBase(/*logResourceLoadErrors*/false)
{
	exemplarTypeToLog = type;
}

void FilteredExemplarLogger::ProcessExemplarResource(
	const char* const originalFunctionName,
	cIGZPersistResource* pResource)
{
	cRZAutoRefCount<cISCPropertyHolder> propertyHolder;

	if (pResource->QueryInterface(GZIID_cISCPropertyHolder, propertyHolder.AsPPVoid()))
	{
		uint32_t type = 0;

		if (SCPropertyUtil::GetPropertyValue(propertyHolder, ExemplarTypePropertyID, type))
		{
			if (type == exemplarTypeToLog)
			{
				LogExemplarTGI(originalFunctionName, pResource);
			}
		}
	}
}
