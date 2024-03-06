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
#include <cstdint>
#include <memory>

class cGZPersistResourceKey;
class cIGZPersistResource;

class ExemplarResourceLoggerBase : public IExemplarLogger
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

protected:

	ExemplarResourceLoggerBase(bool logResourceLoadErrors);

	virtual void ProcessExemplarResource(
		const char* const originalFunctionName,
		cIGZPersistResource* pResource) = 0;

	void LogExemplarTGI(
		const char* const originalFunctionName,
		const cIGZPersistResource* pResource) const;

private:

	bool logResourceLoadErrors;
};