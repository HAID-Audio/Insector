#include "DialogueEditorActions.h"

#define LOCTEXT_NAMESPACE "DialogueEditorCommands"

void FDialogueEditorCommandsImpl::RegisterCommands()
{
	UI_COMMAND(TestCommand, "Test Command", "Test command", EUserInterfaceActionType::Button, FInputChord(EKeys::G, EModifierKey::Shift | EModifierKey::Control))
}

void FDialogueEditorCommands::Register()
{
	return FDialogueEditorCommandsImpl::Register();
}

const FDialogueEditorCommandsImpl& FDialogueEditorCommands::Get()
{
	return FDialogueEditorCommandsImpl::Get();
}

void FDialogueEditorCommands::Unregister()
{
	return FDialogueEditorCommandsImpl::Unregister();
}

#undef LOCTEXT_NAMESPACE