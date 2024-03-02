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

class cGZPersistResourceKey;
class cISCPropertyHolder;

class ExemplarPropertyWriter
{
public:

	ExemplarPropertyWriter()
	{
	}

	virtual ~ExemplarPropertyWriter()
	{
	}

	virtual bool WritePropertyData(
		const char* const originalFunctionName,
		const cGZPersistResourceKey& key,
		cISCPropertyHolder* propertyHolder) = 0;
};

