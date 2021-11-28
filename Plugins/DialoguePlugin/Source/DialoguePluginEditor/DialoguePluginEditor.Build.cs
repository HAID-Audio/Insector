// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class DialoguePluginEditor : ModuleRules
	{
        public DialoguePluginEditor(ReadOnlyTargetRules Target) : base(Target)
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
                    "EditorStyle",
					"Engine",
                    "InputCore",
					"LevelEditor",
					"Slate",
                    "AssetTools",
                    "KismetWidgets",
                    "WorkspaceMenuStructure",
                    "Projects",
                    "GraphEditor",
                    "AnimGraph"
                }
			);

            PrivateDependencyModuleNames.AddRange(
				new string[]
				{					
					"PropertyEditor",
					"SlateCore",
                    "ApplicationCore",
                    "UnrealEd",
                    "Json",
                    "JsonUtilities",
                    "DialoguePlugin"
				}
			);

			PrivateIncludePathModuleNames.AddRange(
				new string[] 
				{
					"MainFrame",
					"Settings"
				}
			);

			
		}
	}
}