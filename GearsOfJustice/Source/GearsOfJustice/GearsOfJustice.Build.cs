// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GearsOfJustice : ModuleRules
{
    public GearsOfJustice(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { 
            "Core", 
            "CoreUObject", 
            "Engine", "InputCore", 
            "NavigationSystem", 
            "AIModule", 
            "Niagara", 
            "EnhancedInput",
            "UMG" 
        });

        PublicIncludePaths.AddRange(new string[]
        {
            "GearsOfJustice/Public/Components",
            "GearsOfJustice/Public/Menu",
            "GearsOfJustice/Public/Player",
            "GearsOfJustice/Public/Dev",
            "GearsOfJustice/Public/Strike"

        });
    }
}
