// Fill out your copyright notice in the Description page of Project Settings.
#define PAPER2D_API

using UnrealBuildTool;

public class Project3Match : ModuleRules
{
	public Project3Match(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Paper2D", "UMG"});
        
        // Uncomment if you are using Slate UI
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
        PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "OnlineSubsystemUtils" });
        //PrivateIncludePathModuleNames.AddRange(new string[] {  });
        DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");
        // Uncomment if you are using online features
        //PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
        if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });
            DynamicallyLoadedModuleNames.Add("IOSAdvertising");
        }
        else if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });
            DynamicallyLoadedModuleNames.Add("AndroidAdvertising");
            // Add UPL to add configrules.txt to our APK
            string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
            AdditionalPropertiesForReceipt.Add("AndroidPlugin", System.IO.Path.Combine(PluginPath, "AddRoundIcon_UPL.xml"));

        }
    }
}
