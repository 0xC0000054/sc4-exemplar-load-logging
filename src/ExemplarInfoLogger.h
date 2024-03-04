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
#include "IExemplarPropertyWriter.h"
#include <cstdint>
#include <memory>

class ExemplarInfoLogger
{
public:

	static ExemplarInfoLogger& GetInstance();

	void SetPropertyWriter(std::unique_ptr<IExemplarPropertyWriter>&& writer);

	void WritePropertyData(
		const char* const originalFunctionName,
		uint32_t riid,
		void** ppvObj);

private:

	std::unique_ptr<IExemplarPropertyWriter> propertyWriter;
	ExemplarInfoLogger();
};