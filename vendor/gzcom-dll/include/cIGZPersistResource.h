#pragma once
#include "cIGZUnknown.h"

class cGZPersistResourceKey;

static const uint32_t GZIID_cIGZPersistResource = 0x456B8F1D;

class cIGZPersistResource : public cIGZUnknown
{
public:

	virtual void GetKey(cGZPersistResourceKey& key) = 0;
	virtual bool SetKey(cGZPersistResourceKey const& key) = 0;

	virtual uint32_t GetType() = 0;
	virtual uint32_t GetGroup() = 0;
	virtual uint32_t GetInstance() = 0;

	virtual bool SetType(uint32_t type) = 0;
	virtual bool SetGroup(uint32_t group) = 0;
	virtual bool SetInstance(uint32_t instance) = 0;

	virtual uint32_t GetRefCount() = 0;
};
