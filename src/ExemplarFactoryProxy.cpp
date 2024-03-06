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

#include "ExemplarFactoryProxy.h"
#include "ExemplarLoggerFactory.h"
#include "Logger.h"
#include "cGZPersistResourceKey.h"
#include "cIGZCOM.h"
#include "cIGZFrameWork.h"
#include "cIGZPersistDBRecord.h"
#include "cRZCOMDllDirector.h"
#include <map>
#include <stdexcept>

static constexpr uint32_t GZCLSID_SCResExemplarFactory = 0x453429b3;

static constexpr uint32_t ExemplarTypeID = 0x6534284A;

namespace
{
	cIGZPersistResourceFactory* CreateOriginalFactory()
	{
		cIGZFrameWork* const pFramework = RZGetFramework();

		cIGZCOM* const pCOM = pFramework->GetCOMObject();

		cIGZPersistResourceFactory* pFactory = nullptr;

		if (!pCOM->GetClassObject(
			GZCLSID_SCResExemplarFactory,
			GZIID_cIGZPersistResourceFactory,
			reinterpret_cast<void**>(&pFactory)))
		{
			throw std::runtime_error("Failed to create the original resource factory.");
		}

		return pFactory;
	}
}

ExemplarFactoryProxy::ExemplarFactoryProxy()
	: refCount(0),
	  originalFactory(CreateOriginalFactory()),
	  exemplarLogger(ExemplarLoggerFactory::GetInstance())
{
}

ExemplarFactoryProxy::~ExemplarFactoryProxy()
{
	if (originalFactory)
	{
		originalFactory->Release();
		originalFactory = nullptr;
	}
}

bool ExemplarFactoryProxy::QueryInterface(uint32_t riid, void** ppvObj)
{
	if (riid == GZIID_cIGZPersistResourceFactory)
	{
		*ppvObj = static_cast<cIGZPersistResourceFactory*>(this);
		AddRef();

		return true;
	}
	else if (riid == GZIID_cIGZUnknown)
	{
		*ppvObj = static_cast<cIGZUnknown*>(this);
		AddRef();

		return true;
	}

	*ppvObj = nullptr;
	return false;
}

uint32_t ExemplarFactoryProxy::AddRef()
{
	originalFactory->AddRef();

	++refCount;

	originalFactory->Release();

	return refCount;
}

uint32_t ExemplarFactoryProxy::Release()
{
	uint32_t localRefCount = 0;

	originalFactory->AddRef();

	if (refCount == 1)
	{
		// After this Release call the object will have a reference count of 1.
		// When the class destructor runs it will release the final reference and
		// set the pointer to null.
		originalFactory->Release();

		delete this;
		localRefCount = 0;
	}
	else
	{
		localRefCount = refCount - 1;
		refCount = localRefCount;

		originalFactory->Release();
	}

	return localRefCount;
}

bool ExemplarFactoryProxy::CreateInstance(
	uint32_t type,
	uint32_t riid,
	void** ppvObj,
	uint32_t unknown1,
	cIGZUnknown* unknown2)
{
	if (type == ExemplarTypeID)
	{
		bool result = originalFactory->CreateInstance(type, riid, ppvObj, unknown1, unknown2);

		if (exemplarLogger)
		{
			if (result)
			{
				exemplarLogger->ExemplarLoaded(__FUNCSIG__, riid, ppvObj);
			}
			else
			{
				exemplarLogger->LoadError(__FUNCSIG__, riid);
			}
		}

		return result;
	}

	*ppvObj = nullptr;
	return false;
}

bool ExemplarFactoryProxy::CreateInstance(
	cIGZPersistDBRecord& record,
	uint32_t riid,
	void** ppvObj,
	uint32_t unknown1,
	cIGZUnknown* unknown2)
{
	cGZPersistResourceKey key;
	record.GetKey(key);

	if (key.type == ExemplarTypeID)
	{
		bool result = originalFactory->CreateInstance(record, riid, ppvObj, unknown1, unknown2);

		if (exemplarLogger)
		{
			if (result)
			{
				exemplarLogger->ExemplarLoaded(__FUNCSIG__, riid, ppvObj);
			}
			else
			{
				cGZPersistResourceKey key;
				record.GetKey(key);

				exemplarLogger->LoadError(__FUNCSIG__, riid, key);
			}
		}

		return result;
	}

	*ppvObj = nullptr;
	return false;
}

bool ExemplarFactoryProxy::Read(cIGZPersistResource& resource, cIGZPersistDBRecord& record)
{
	bool result = originalFactory->Read(resource, record);

	Logger::GetInstance().WriteLineFormatted(
		LogLevel::Debug,
		__FUNCSIG__ ": result=%s",
		result ? "true" : "false");

	return result;
}

bool ExemplarFactoryProxy::Write(cIGZPersistResource const& resource, cIGZPersistDBRecord& record)
{
	bool result = originalFactory->Write(resource, record);

	Logger::GetInstance().WriteLineFormatted(
		LogLevel::Debug,
		__FUNCSIG__ ": result=%s",
		result ? "true" : "false");

	return result;
}
