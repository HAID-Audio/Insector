#pragma once
#include "CoreMinimal.h"
#include "IDialoguePluginEditorModule.h"
#include "Runtime/SlateCore/Public/Styling/SlateStyleRegistry.h"

#define LOCTEXT_NAMESPACE "DialoguePluginEditor"

class FDialoguePluginEditorModule : public IDialoguePluginEditorModule
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** IDialoguePluginEditorModule implementation */
	virtual bool AllowAutomaticGraphicsSwitching() override;
	virtual bool AllowMultipleGPUs() override;

	static uint32 GameAssetCategory;
	
private:
	void Initialize( TSharedPtr<class SWindow> InRootWindow, bool bIsNewProjectWindow );
	void AddGraphicsSwitcher( class FToolBarBuilder& ToolBarBuilder );
	
private:
	void RegisterAssetTypeAction(class IAssetTools& AssetTools, TSharedRef<class IAssetTypeActions> Action);
	TSharedPtr< class FExtender > NotificationBarExtender;
	bool bAllowAutomaticGraphicsSwitching;
	bool bAllowMultiGPUs;
};

#undef LOCTEXT_NAMESPACE