// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HotFixLua : ModuleRules
{
	public HotFixLua(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core","PakFile","Unlua","CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
        OptimizeCode = CodeOptimization.InShippingBuildsOnly;
    }
}
