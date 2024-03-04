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

#include "ExemplarTypePropertyWriter.h"
#include "Logger.h"
#include "cGZPersistResourceKey.h"
#include "cIGZVariant.h"
#include "cISCProperty.h"
#include "cISCPropertyHolder.h"
#include <cstdint>
#include <map>

static constexpr uint32_t ExemplarTypePropertyID = 0x00000010;

static const std::map<uint32_t, std::string> ExemplarTypeMap
{
	{ 0x00000000, "Other/Unknown" },
	{ 0x00000001, "Tuning" },
	{ 0x00000002, "Buildings" },
	{ 0x00000003, "RCI" },
	{ 0x00000004, "Developer" },
	{ 0x00000005, "Simulator" },
	{ 0x00000006, "Road" },
	{ 0x00000007, "Bridge" },
	{ 0x00000008, "MiscNetwork" },
	{ 0x00000009, "NetworkIntersection" },
	{ 0x0000000A, "Rail" },
	{ 0x0000000B, "Highway" },
	{ 0x0000000C, "PowerLine" },
	{ 0x0000000D, "Terrain" },
	{ 0x0000000E, "Ordinances" },
	{ 0x0000000F, "Flora" },
	{ 0x00000010, "LotConfigurations" },
	{ 0x00000011, "Foundations" },
	{ 0x00000012, "Advice" },
	{ 0x00000013, "Lighting" },
	{ 0x00000014, "Cursor" },
	{ 0x00000015, "LotRetainingWalls" },
	{ 0x00000016, "Vehicles" },
	{ 0x00000017, "Pedestrians" },
	{ 0x00000018, "Aircraft" },
	{ 0x00000019, "Watercraft" },
	{ 0x0000001E, "Prop" },
	{ 0x0000001F, "Construction" },
	{ 0x00000020, "Automata Tuning" },
	{ 0x00000021, "Type 21" },
	{ 0x00000022, "Disaster" },
	{ 0x00000023, "Data view" },
	{ 0x00000024, "Crime" },
	{ 0x00000025, "Audio" },
	{ 0x00000026, "My Sim Template" },
	{ 0x00000027, "TerrainBrush" },
	{ 0x00000028, "Misc Catalog" },
};

namespace
{
	bool GetPropertyValue(const cISCPropertyHolder* propertyHolder, uint32_t id, uint32_t& value)
	{
		if (propertyHolder)
		{
			const cISCProperty* property = propertyHolder->GetProperty(id);

			if (property)
			{
				const cIGZVariant* data = property->GetPropertyValue();

				if (data)
				{
					return data->GetValUint32(value);
				}
			}
		}

		value = 0;
		return false;
	}

	const char* const GetExemplarTypeName(uint32_t type)
	{
		const auto& item = ExemplarTypeMap.find(type);

		if (item != ExemplarTypeMap.end())
		{
			return item->second.c_str();
		}

		return "Unknown";
	}
}

ExemplarTypePropertyWriter::ExemplarTypePropertyWriter()
{
}

bool ExemplarTypePropertyWriter::WritePropertyData(
	const char* const originalFunctionName,
	const cGZPersistResourceKey& key,
	const cISCPropertyHolder* propertyHolder)
{
	Logger& logger = Logger::GetInstance();

	uint32_t exemplarType = 0;

	if (GetPropertyValue(propertyHolder, ExemplarTypePropertyID, exemplarType))
	{
		if (logger.IsEnabled(LogLevel::Debug))
		{
			logger.WriteLineFormatted(
				LogLevel::Debug,
				"%s: T=0x%08X G=0x%08X, I=0x%08X, ExemplarType=%s",
				originalFunctionName,
				key.type,
				key.group,
				key.instance,
				GetExemplarTypeName(exemplarType));
		}
		else
		{
			logger.WriteLineFormatted(
				LogLevel::Info,
				"T=0x%08X G=0x%08X, I=0x%08X, ExemplarType=%s",
				key.type,
				key.group,
				key.instance,
				GetExemplarTypeName(exemplarType));
		}

		return true;
	}

	return false;
}
