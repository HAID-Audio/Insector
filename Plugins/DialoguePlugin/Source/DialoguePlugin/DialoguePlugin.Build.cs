// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class DialoguePlugin : ModuleRules
	{
        public DialoguePlugin(ReadOnlyTargetRules Target) : base(Target)
        {
            PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

            PublicIncludePaths.AddRange(new string[]
			{
                System.IO.Path.Combine(ModuleDirectory, "Public")
			});


            PrivateIncludePaths.AddRange(new string[] 
			{ 
				System.IO.Path.Combine(ModuleDirectory, "Private")
			});

            PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
                  	"Engine",
                    "SlateCore",
                    "UMG"
                }
			);
			
		}
	}
}