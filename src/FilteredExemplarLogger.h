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

#pragma once
#include "ExemplarResourceLoggerBase.h"

// Filters the exemplars by their ExemplarType and logs the TGI
// of any matching exemplars.

class FilteredExemplarLogger final : public ExemplarResourceLoggerBase
{
public:

	FilteredExemplarLogger(uint32_t type);

private:

	void ProcessExemplarResource(
		const char* const originalFunctionName,
		cIGZPersistResource* pResource) override;

	uint32_t exemplarTypeToLog;
};

