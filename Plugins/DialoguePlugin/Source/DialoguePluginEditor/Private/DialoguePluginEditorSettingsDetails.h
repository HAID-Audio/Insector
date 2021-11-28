#pragma once

#include "Editor/PropertyEditor/Public/IDetailCustomization.h"
#include "PropertyCustomizationHelpers.h"

class UDialogue;

class FDialoguePluginEditorSettingsDetails : public IDetailCustomization
{
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IDetailCustomization> MakeInstance();
	/** IDetailCustomization interface */
	virtual void CustomizeDetails( IDetailLayoutBuilder& DetailLayout ) override;	
	void TextCommited(const FText& InText, ETextCommit::Type InCommitType, UDialogue* Dialogue, int32 id);

private:
	//FReply RefreshOnClicked();

	/** Associated detail layout builder */
	IDetailLayoutBuilder* DetailLayoutBuilder;
};
