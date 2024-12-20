// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EDR : ModuleRules
{
	public EDR(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore","UMG", "EnhancedInput", "NavigationSystem","AIModule","GameplayTasks" });
    }
}
