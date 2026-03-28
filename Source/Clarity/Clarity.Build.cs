// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Clarity : ModuleRules
{
	public Clarity(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"Clarity",
			"Clarity/Variant_Platforming",
			"Clarity/Variant_Platforming/Animation",
			"Clarity/Variant_Combat",
			"Clarity/Variant_Combat/AI",
			"Clarity/Variant_Combat/Animation",
			"Clarity/Variant_Combat/Gameplay",
			"Clarity/Variant_Combat/Interfaces",
			"Clarity/Variant_Combat/UI",
			"Clarity/Variant_SideScrolling",
			"Clarity/Variant_SideScrolling/AI",
			"Clarity/Variant_SideScrolling/Gameplay",
			"Clarity/Variant_SideScrolling/Interfaces",
			"Clarity/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
