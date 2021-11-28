#include "DialoguePluginModule.h"
#include "DialoguePluginPrivatePCH.h"
#include "Runtime/Core/Public/Features/IModularFeatures.h"
#include "Runtime/SlateCore/Public/Rendering/SlateRenderer.h"
#include "Editor/MainFrame/Public/Interfaces/IMainFrameModule.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "DialoguePlugin"

class FDialoguePluginModule : public IDialoguePluginModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
private:

	void Initialize( TSharedPtr<SWindow> InRootWindow, bool bIsNewProjectWindow );
	void AddGraphicsSwitcher( FToolBarBuilder& ToolBarBuilder );
	TSharedPtr< FExtender > NotificationBarExtender;
	bool bAllowAutomaticGraphicsSwitching;
	bool bAllowMultiGPUs;
};

IMPLEMENT_MODULE(FDialoguePluginModule, DialoguePlugin)

void FDialoguePluginModule::StartupModule()
{	
	
}

void FDialoguePluginModule::Initialize( TSharedPtr<SWindow> InRootWindow, bool bIsNewProjectWindow )
{

}

void FDialoguePluginModule::AddGraphicsSwitcher( FToolBarBuilder& ToolBarBuilder )
{
	
}

void FDialoguePluginModule::ShutdownModule()
{

}

#undef LOCTEXT_NAMESPACE
