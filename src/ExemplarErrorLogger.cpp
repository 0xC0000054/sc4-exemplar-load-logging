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

#include "ExemplarErrorLogger.h"
#include "Logger.h"
#include "cGZPersistResourceKey.h"

void ExemplarErrorLogger::ExemplarLoaded(
	const char* const originalFunctionName,
	uint32_t riid,
	void** ppvObj)
{
}

void ExemplarErrorLogger::LoadError(
	const char* const originalFunctionName,
	uint32_t riid)
{
	LogErrorStatic(originalFunctionName, riid);
}

void ExemplarErrorLogger::LoadError(
	const char* const originalFunctionName,
	uint32_t riid,
	const cGZPersistResourceKey& key)
{
	LogErrorStatic(originalFunctionName, riid, key);
}

void ExemplarErrorLogger::LogErrorStatic(
	const char* const originalFunctionName,
	uint32_t riid)
{
	Logger& logger = Logger::GetInstance();

	if (logger.IsEnabled(LogLevel::Debug))
	{
		logger.WriteLineFormatted(
			LogLevel::Info,
			__FUNCSIG__ ": Error loading exemplar, riid=0x%08X",
			riid);
	}
	else
	{
		logger.WriteLineFormatted(
			LogLevel::Info,
			"Error loading exemplar, riid=0x%08X",
			riid);
	}
}

void ExemplarErrorLogger::LogErrorStatic(
	const char* const originalFunctionName,
	uint32_t riid,
	const cGZPersistResourceKey& key)
{
	Logger& logger = Logger::GetInstance();

	if (logger.IsEnabled(LogLevel::Debug))
	{
		logger.WriteLineFormatted(
			LogLevel::Info,
			__FUNCSIG__ ": Error loading exemplar, T=0x%08X G=0x%08X, I=0x%08X, riid=0x%08X",
			key.type,
			key.group,
			key.instance,
			riid);
	}
	else
	{
		logger.WriteLineFormatted(
			LogLevel::Info,
			"Error loading exemplar, T=0x%08X G=0x%08X, I=0x%08X, riid=0x%08X",
			key.type,
			key.group,
			key.instance,
			riid);
	}
}
