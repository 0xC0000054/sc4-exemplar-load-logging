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

#include "ExemplarLoggerFactory.h"

static std::shared_ptr<IExemplarLogger> instance;

std::shared_ptr<IExemplarLogger> ExemplarLoggerFactory::GetInstance()
{
	return instance;
}

void ExemplarLoggerFactory::SetInstance(std::shared_ptr<IExemplarLogger>&& logger)
{
	instance = std::move(logger);
}
