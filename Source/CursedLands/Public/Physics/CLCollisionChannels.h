// Copyright Anton Vasserman, All Rights Reserved.

#pragma once

// DefaultEngine.ini [/Script/Engine.CollisionProfile] should match with this list

// TODO: Create a Macro for generating those Channels and consider moving it to the Utility Plugin
// Trace against Actors which can be traversed over.
#define CL_TraceChannel_Traversability		ECC_GameTraceChannel1
#define CL_TraceTypeQuery_Traversability	ETraceTypeQuery::TraceTypeQuery15