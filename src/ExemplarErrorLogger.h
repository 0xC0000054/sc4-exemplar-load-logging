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
#include "IExemplarLogger.h"

class ExemplarErrorLogger final : public IExemplarLogger
{
public:

	void ExemplarLoaded(
		const char* const originalFunctionName,
		uint32_t riid,
		void** ppvObj) override;

	void LoadError(
		const char* const originalFunctionName,
		uint32_t riid) override;

	void LoadError(
		const char* const originalFunctionName,
		uint32_t riid,
		const cGZPersistResourceKey& key) override;

	static void LogErrorStatic(
		const char* const originalFunctionName,
		uint32_t riid);

	static void LogErrorStatic(
		const char* const originalFunctionName,
		uint32_t riid,
		const cGZPersistResourceKey& key);
};

