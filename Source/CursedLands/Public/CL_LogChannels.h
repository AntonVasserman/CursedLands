// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

#include "Logging/LogMacros.h"

class UObject;

CURSEDLANDS_API DECLARE_LOG_CATEGORY_EXTERN(LogCL, Log, All);

#define CL_LOG(LogLevel, Format, ...) UE_LOG(LogCL, LogLevel, TEXT("%s: " Format), __FUNCTIONW__, ##__VA_ARGS__)

CURSEDLANDS_API DECLARE_LOG_CATEGORY_EXTERN(LogCLGameplayAbilitySystem, Log, All);

#define CL_LOG_GAMEPLAY_ABILITY_SYSTEM(LogLevel, Format, ...) UE_LOG(LogCLGameplayAbilitySystem, LogLevel, TEXT("%s: " Format), __FUNCTIONW__, ##__VA_ARGS__)

#define CL_VARIABLE_NAME(Variable) TEXT(#Variable)
