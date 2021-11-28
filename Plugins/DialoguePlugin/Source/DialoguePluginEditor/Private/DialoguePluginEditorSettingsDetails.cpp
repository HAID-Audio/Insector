#include "DialoguePluginEditorSettingsDetails.h"
#include "DialoguePluginEditorPrivatePCH.h"
#include "Dialogue.h"
#include "DialogueViewportWidget.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Editor/PropertyEditor/Public/DetailLayoutBuilder.h"
#include "Editor/PropertyEditor/Public/DetailCategoryBuilder.h"
#include "Editor/UnrealEd/Public/ScopedTransaction.h"
#include "Internationalization/Text.h"


#define LOCTEXT_NAMESPACE "DialoguePluginSettingsDetails"

TSharedRef<IDetailCustomization> FDialoguePluginEditorSettingsDetails::MakeInstance()
{
	return MakeShareable(new FDialoguePluginEditorSettingsDetails());
}

void FDialoguePluginEditorSettingsDetails::CustomizeDetails( IDetailLayoutBuilder& DetailLayout )
{
	DetailLayoutBuilder = &DetailLayout;

	const TSharedPtr<IPropertyHandle> DataProperty = DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(UDialogue, Data));
	DetailLayout.HideProperty(DataProperty);
	const TSharedPtr<IPropertyHandle> NextNodeProperty = DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(UDialogue, NextNodeId));
	DetailLayout.HideProperty(NextNodeProperty);

	// Create a category so this is displayed early in the properties
	IDetailCategoryBuilder& MyCategory = DetailLayout.EditCategory("Dialogue Editor");
	IDetailCategoryBuilder& CurrentNodeCategory = DetailLayout.EditCategory("Current Node", LOCTEXT("CurrentNode", "Current Node"), ECategoryPriority::Important);

	TArray<TWeakObjectPtr<UObject>> ObjectsBeingCustomized;

	DetailLayout.GetObjectsBeingCustomized(ObjectsBeingCustomized);
	TArray<UObject*> StrongObjects;
	CopyFromWeakArray(StrongObjects, ObjectsBeingCustomized);

	if (StrongObjects.Num() == 0) return;

	UDialogue* Dialogue = Cast<UDialogue>(StrongObjects[0]);
	
	if (Dialogue->CurrentNodeId != -1 && Dialogue->CurrentNodeId != 0) //display current node details:
	{
		int32 index;
		FDialogueNode CurrentNode = Dialogue->GetNodeById(Dialogue->CurrentNodeId, index);

		CurrentNodeCategory.AddCustomRow(LOCTEXT("Text", "Text"))
			.WholeRowContent()
			[
				SNew(STextBlock).Font(IDetailLayoutBuilder::GetDetailFont())
				.Text(FText::Format(LOCTEXT("TextRowWithId", "Text (node id: {0})"), Dialogue->CurrentNodeId)) // displays current Node Id in inspector
			];

		CurrentNodeCategory.AddCustomRow(LOCTEXT("TextValue", "TextValue"))
			.WholeRowContent()
			[
				SNew(SBox)
				.HeightOverride(100)
				[
					SNew(SMultiLineEditableTextBox).Text(CurrentNode.Text)
					.AutoWrapText(true)
					.OnTextCommitted(this, &FDialoguePluginEditorSettingsDetails::TextCommited, Dialogue, Dialogue->CurrentNodeId)
					.ModiferKeyForNewLine(EModifierKey::Shift)
				]
				
			];
			
		const TSharedPtr<IPropertyHandleArray> Array = DataProperty->AsArray();
		const TSharedPtr<IPropertyHandle> Child = Array->GetElement(index);
		const TSharedPtr<IPropertyHandle> IsPlayerField = Child->GetChildHandle("isPlayer");
		const TSharedPtr<IPropertyHandle> DrawCommentBubble = Child->GetChildHandle("bDrawBubbleComment");
		const TSharedPtr<IPropertyHandle> Comment = Child->GetChildHandle("BubbleComment");
		const TSharedPtr<IPropertyHandle> EventsField = Child->GetChildHandle("Events");
		const TSharedPtr<IPropertyHandle> ConditionsField = Child->GetChildHandle("Conditions");
		const TSharedPtr<IPropertyHandle> SoundField = Child->GetChildHandle("Sound");
		const TSharedPtr<IPropertyHandle> DialogueWaveField = Child->GetChildHandle("DialogueWave");
		
		CurrentNodeCategory.AddProperty(IsPlayerField);
		CurrentNodeCategory.AddProperty(DrawCommentBubble);
		CurrentNodeCategory.AddProperty(Comment);
		CurrentNodeCategory.AddProperty(EventsField);		
		
		/* 
		* Customizing Conditions Row
		*/
		IDetailPropertyRow * ConditionDetailsRow = &CurrentNodeCategory.AddProperty(ConditionsField);

		//TSharedPtr<SWidget> DefaultNameWidget;
		//TSharedPtr<SWidget> DefaultValueWidget;
		//FDetailWidgetRow DefaultWidgetRow;
		//ConditionDetailsRow->GetDefaultWidgets(DefaultNameWidget, DefaultValueWidget, DefaultWidgetRow);
		//
		//FDetailWidgetRow & CustomRow = ConditionDetailsRow->CustomWidget(false); // erases the default contents of the row
		//CustomRow.NameContent()
		//[
		//	DefaultNameWidget.ToSharedRef()
		//]
		//.ValueContent()
		//.MinDesiredWidth(170.0f)
		//[
		//	DefaultValueWidget.ToSharedRef()
		//];

		//uint32 children = 0;
		//ConditionsField->GetNumChildren(children);
		//for (uint32 i = 0; i < children; i++)
		//{
		//	TSharedPtr<IPropertyHandle> childHandle = ConditionsField->GetChildHandle(i);
		//	IDetailPropertyRow * subConditionRow = &CurrentNodeCategory.AddProperty(childHandle);
		//	subConditionRow->ShouldAutoExpand(true);
		//}
		/*
		* end of conditions customization
		*/

		CurrentNodeCategory.AddProperty(SoundField);
		CurrentNodeCategory.AddProperty(DialogueWaveField);		
	}	
	
}

void FDialoguePluginEditorSettingsDetails::TextCommited(const FText& NewText, ETextCommit::Type CommitInfo, UDialogue* Dialogue, int32 id)
{
	int32 index;
	FDialogueNode CurrentNode = Dialogue->GetNodeById(id, index);

	// we don't commit text if it hasn't changed
	if (Dialogue->Data[index].Text.EqualTo(NewText))
	{
		return;
	}
	
	const FScopedTransaction Transaction(LOCTEXT("TextCommited", "Edited Node Text"));
	Dialogue->Modify();
	
	TOptional<FString> keyOpt = FTextInspector::GetKey(Dialogue->Data[index].Text);
	TOptional<FString> nsOpt = FTextInspector::GetNamespace(Dialogue->Data[index].Text);
	Dialogue->Data[index].Text = FText::ChangeKey(
		FTextKey(nsOpt.IsSet() ? nsOpt.GetValue() : ""),
		FTextKey(keyOpt.IsSet() ? keyOpt.GetValue() : ""),
		NewText
	);
}

#undef LOCTEXT_NAMESPACE
