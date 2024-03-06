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

class ExemplarTypeLogger final : public ExemplarResourceLoggerBase
{
public:

	ExemplarTypeLogger(bool logResourceLoadErrors = false);

private:

	void ProcessExemplarResource(
		const char* const originalFunctionName,
		cIGZPersistResource* pResource) override;
};

