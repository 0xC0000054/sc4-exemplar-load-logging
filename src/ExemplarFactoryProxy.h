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
#include "cIGZPersistResourceFactory.h"
#include <memory>

static constexpr uint32_t GZCLSID_ExemplarFactoryProxy = 0xEDA309D9;

/**
 * @brief A class that logs the exemplar info as it is loaded by SC4's resource system.
 *
 * @remarks This class works by forwarding the requests to SC4's built-in exemplar
 * factory class and logging the result of that call.
 */
class ExemplarFactoryProxy : public cIGZPersistResourceFactory
{
public:

	ExemplarFactoryProxy();

	virtual ~ExemplarFactoryProxy();

	bool QueryInterface(uint32_t riid, void** ppvObj);

	uint32_t AddRef();

	uint32_t Release();

	bool CreateInstance(
		uint32_t type,
		uint32_t riid,
		void** ppvObj,
		uint32_t unknown1,
		cIGZUnknown* unknown2);
	bool CreateInstance(
		cIGZPersistDBRecord& record,
		uint32_t riid,
		void** ppvObj,
		uint32_t unknown1,
		cIGZUnknown* unknown2);

	bool Read(cIGZPersistResource& resource, cIGZPersistDBRecord& record);
	bool Write(cIGZPersistResource const& resource, cIGZPersistDBRecord& record);

private:

	uint32_t refCount;
	cIGZPersistResourceFactory* originalFactory;

	std::shared_ptr<IExemplarLogger> exemplarLogger;
};

