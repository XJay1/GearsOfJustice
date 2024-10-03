// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GearsOfJustice : ModuleRules
{
    public GearsOfJustice(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput" });
        PublicIncludePaths.AddRange(new string[]
        {
            "GearsOfJustice/Public",
            "GearsOfJustice/Public/Menu",
            "GearsOfJustice/Public/Menu/UI"
        });

    }
}