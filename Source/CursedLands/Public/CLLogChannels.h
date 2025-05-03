// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "Logging/LogMacros.h"

class UObject;

CURSEDLANDS_API DECLARE_LOG_CATEGORY_EXTERN(LogCL, Log, All);
CURSEDLANDS_API DECLARE_LOG_CATEGORY_EXTERN(LogCLGameplayAbilitySystem, Log, All);

// TODO (CL-121):	Create a custom UE_LOG that can output the __FUNCTION__ first and then the rest of the log
//					Consider it being a Utility in AVUtilities?
#define CL_LOG(CategoryName, Verbosity, Format, ...) UE_LOG(CategoryName, Verbosity, Format, ##__VA_ARGS__)