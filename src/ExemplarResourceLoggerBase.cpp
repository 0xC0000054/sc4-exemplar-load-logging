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

#include "ExemplarResourceLoggerBase.h"
#include "ExemplarErrorLogger.h"
#include "Logger.h"
#include "cGZPersistResourceKey.h"
#include "cIGZPersistResource.h"
#include "cISCPropertyHolder.h"
#include "cRZAutoRefCount.h"

ExemplarResourceLoggerBase::ExemplarResourceLoggerBase(bool logResourceLoadErrors)
	: logResourceLoadErrors(logResourceLoadErrors)
{
}

void ExemplarResourceLoggerBase::ExemplarLoaded(
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

		ProcessExemplarResource(originalFunctionName, pRes);
	}
	else
	{
		if (logger.IsEnabled(LogLevel::Debug))
		{
			logger.WriteLineFormatted(
				LogLevel::Error,
				"%s: The object is null or not cIGZPersistResource, riid=0x%08X",
				originalFunctionName,
				riid);
		}
		else
		{
			logger.WriteLineFormatted(
				LogLevel::Error,
				"The object is null or not cIGZPersistResource, riid=0x%08X",
				originalFunctionName);
		}
	}
}

void ExemplarResourceLoggerBase::LoadError(
	const char* const originalFunctionName,
	uint32_t riid)
{
	if (logResourceLoadErrors)
	{
		ExemplarErrorLogger::LogErrorStatic(originalFunctionName, riid);
	}
}

void ExemplarResourceLoggerBase::LoadError(const char* const originalFunctionName, uint32_t riid, const cGZPersistResourceKey& key)
{
	if (logResourceLoadErrors)
	{
		ExemplarErrorLogger::LogErrorStatic(originalFunctionName, riid, key);
	}
}

void ExemplarResourceLoggerBase::LogExemplarTGI(
	const char* const originalFunctionName,
	const cIGZPersistResource* pResource) const
{
	Logger& logger = Logger::GetInstance();

	cGZPersistResourceKey key;
	pResource->GetKey(key);

	if (logger.IsEnabled(LogLevel::Debug))
	{
		logger.WriteLineFormatted(
			LogLevel::Debug,
			"%s: T=0x%08X G=0x%08X, I=0x%08X",
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
