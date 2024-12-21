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
            "Engine", 
            "InputCore", 
            "NavigationSystem", 
            "AIModule", 
            "Niagara", 
            "EnhancedInput",
            "UMG", 
            "GameplayTasks",
            "NavigationSystem" 
        });

        PublicIncludePaths.AddRange(new string[]
        {
            "GearsOfJustice/Public/Components",
            "GearsOfJustice/Public/Menu",
            "GearsOfJustice/Public/Player",
            "GearsOfJustice/Public/Dev",
            "GearsOfJustice/Public/Strike",
            "GearsOfJustice/Public/AI",
            "GearsOfJustice/Public/AI/Service",
            "GearsOfJustice/Public/AI/Decorator",
            "GearsOfJustice/Public/Animation"

        });
    }
}
