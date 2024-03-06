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

#include "ExemplarTGILogger.h"

ExemplarTGILogger::ExemplarTGILogger(bool logResourceLoadErrors)
	: ExemplarResourceLoggerBase(logResourceLoadErrors)
{
}

void ExemplarTGILogger::ProcessExemplarResource(
	const char* const originalFunctionName,
	cIGZPersistResource* pResource)
{
	LogExemplarTGI(originalFunctionName, pResource);
}
