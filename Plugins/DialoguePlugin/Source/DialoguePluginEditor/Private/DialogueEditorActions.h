#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "EditorStyleSet.h"

class FDialogueEditorCommandsImpl : public TCommands<FDialogueEditorCommandsImpl>
{
public:

	FDialogueEditorCommandsImpl()
		: TCommands<FDialogueEditorCommandsImpl>(TEXT("DialogueEditor"), NSLOCTEXT("Contexts", "DialogueEditor", "Dialogue Editor"), NAME_None, FEditorStyle::GetStyleSetName())
	{
	}

	virtual ~FDialogueEditorCommandsImpl()
	{
	}

	DIALOGUEPLUGINEDITOR_API virtual void RegisterCommands() override;

	TSharedPtr< FUICommandInfo > TestCommand;
};

class DIALOGUEPLUGINEDITOR_API FDialogueEditorCommands
{
public:
	static void Register();

	static const FDialogueEditorCommandsImpl& Get();

	static void Unregister();
};
