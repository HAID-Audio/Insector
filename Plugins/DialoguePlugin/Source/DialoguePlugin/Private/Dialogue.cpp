#include "Dialogue.h"
#include "Runtime/Core/Public/Misc/OutputDeviceNull.h"
#include "Engine/GameInstance.h"
#include "DialoguePluginPrivatePCH.h"



UDialogue::UDialogue(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

void UDialogue::AssignPersistentOuter(UGameInstance * inGameInstance)
{
	PersistentGameInstance = inGameInstance;
}

void UDialogue::CleanOuter()
{
	PersistentGameInstance = nullptr;
}

UWorld* UDialogue::GetWorld() const
{	
	if (PersistentGameInstance)
	{
		return PersistentGameInstance->GetWorld();
	}
	else
	{
		return nullptr;
	}
}

FDialogueNode UDialogue::GetNodeById(int32 id, int32 & index)
{
	index = -1;

	int i = 0;
	for (FDialogueNode FoundNode : Data)
	{
		if (FoundNode.id == id)
		{
			index = i;
			return FoundNode;
		}
		i++;
	}

	FDialogueNode Empty;
	return Empty;
}

FDialogueNode UDialogue::GetNodeById(int32 id)
{
	int32 index;
	return GetNodeById(id, index);
}

// added later for BP usage, not used in C++ by the plugin
FDialogueNode UDialogue::GetNodeById(int32 id, bool& found)
{
	int32 index;
	FDialogueNode result = GetNodeById(id, index);
	found = index != -1;
	return result;
}

FDialogueNode UDialogue::GetFirstNode()
{
	FDialogueNode StartNode = GetNodeById(0);

	if (StartNode.Links.Num() > 0)
	{
		return GetNodeById(StartNode.Links[0]);
	}

	FDialogueNode Empty;
	return Empty;
}

TArray<FDialogueNode> UDialogue::GetNextNodes(FDialogueNode Node)
{
	TArray<FDialogueNode> Output;

	for (int32 foundindex : Node.Links)
	{
		Output.Add(GetNodeById(foundindex));
	}

	return Output;
}

void UDialogue::CallFunctionByName(UObject* Object, FString FunctionName)
{
	FOutputDeviceNull ar;
	Object->CallFunctionByNameWithArguments(*FunctionName, ar, NULL, true);
}

UDialogueConditions::UDialogueConditions() 
{

}

UWorld* UDialogueConditions::GetWorld() const
{
	UDialogue * outerDialogue = GetTypedOuter<UDialogue>();

	if (outerDialogue)
	{
		return outerDialogue->GetWorld();
	}
	else
	{
		return nullptr;
	}
}

UDialogueEvents::UDialogueEvents()
{

}

void UDialogueEvents::RecieveEventTriggered_Implementation(APlayerController* ConsideringPlayer, AActor* NPCActor)
{
	return;
}

bool UDialogueConditions::IsConditionMet_Implementation(APlayerController* ConsideringPlayer, AActor* NPCActor)
{
	return IsConditionMet_Internal(ConsideringPlayer, NPCActor);
}

UWorld* UDialogueEvents::GetWorld() const
{
	UDialogue * outerDialogue = GetTypedOuter<UDialogue>();

	if (outerDialogue)
	{
		return outerDialogue->GetWorld();
	}
	else
	{
		return nullptr;
	}
}

#if WITH_EDITOR
/*
* If we add a comment to a node, but bDrawBubbleComment is false, set it to true so it's instantly displayed.
*/
void UDialogue::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetPropertyName().ToString().Equals(TEXT("BubbleComment"), ESearchCase::CaseSensitive))
	{
		if (CurrentNodeId < 0)
		{
			return;
		}
		int32 index;
		FDialogueNode selectedNodeCopy = GetNodeById(CurrentNodeId, index);		
		// if bDrawBubble is false, but BobbleComment contains text
		if (!selectedNodeCopy.bDrawBubbleComment && !selectedNodeCopy.BubbleComment.EqualTo(FText::GetEmpty()))
		{
			Data[index].bDrawBubbleComment = true;
		}
		// if bDrawBubble is true, but BobbleComment is empty
		if (selectedNodeCopy.bDrawBubbleComment && selectedNodeCopy.BubbleComment.EqualTo(FText::GetEmpty()))
		{
			Data[index].bDrawBubbleComment = false;
		}
	}
}
#endif