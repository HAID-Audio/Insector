#include "DialogueAssetTypeActions.h"
#include "DialoguePluginEditorPrivatePCH.h"
#include "DialogueEditor.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"


FDialogueAssetTypeActions::FDialogueAssetTypeActions(uint32 InAssetCategory)
	: MyAssetCategory(InAssetCategory)
{
}

FText FDialogueAssetTypeActions::GetName() const
{
	return LOCTEXT("FDialogueAssetTypeActionsName", "Dialogue");
}

FColor FDialogueAssetTypeActions::GetTypeColor() const
{
	return FColor(255, 55, 220);
}

UClass* FDialogueAssetTypeActions::GetSupportedClass() const
{
	return UDialogue::StaticClass();
}

void FDialogueAssetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		if (UDialogue* Dialogue = Cast<UDialogue>(*ObjIt))
		{
			TSharedRef<FDialogueEditor> NewDialogueEditor(new FDialogueEditor());
			NewDialogueEditor->InitDialogueEditor(Mode, EditWithinLevelEditor, Dialogue);
		}
	}
}

uint32 FDialogueAssetTypeActions::GetCategories()
{
	return MyAssetCategory; //affects filters
}

#undef LOCTEXT_NAMESPACE
