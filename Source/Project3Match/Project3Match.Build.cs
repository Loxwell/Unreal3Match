// Fill out your copyright notice in the Description page of Project Settings.
#define PAPER2D_API

using UnrealBuildTool;

public class Project3Match : ModuleRules
{
	public Project3Match(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Paper2D" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Paper2D" });
        PrivateIncludePathModuleNames.AddRange(new string[] { @"Paper2D" });
		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
