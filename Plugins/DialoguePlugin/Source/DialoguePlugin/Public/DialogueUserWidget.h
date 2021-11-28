// Copyright Underflow Studios 2017

#pragma once

#include "CoreMinimal.h"
#include "Dialogue.h"
#include "Blueprint/UserWidget.h"
#include "DialogueUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUEPLUGIN_API UDialogueUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// Conditions
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Dialogue UI")
	bool IsConditionsMetForNode(FDialogueNode Node);

	// Events
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Dialogue UI")
	void RunEventsForNode(FDialogueNode Node);

	//VarStrings
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintPure, Category = "Dialogue UI")
	FText ReplaceVarStrings(const FText & InText);

	//VarStrings
	UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
	bool RunStringReplacer(FString originalString, FString& resultString);

	//VarStrings
	UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
	TArray<FString> FindVarStrings(FText inText);

	/* This function is only useful for the TypeWriter effect.
	Usage: call the function in a loop with delays, feeding it incrementing steps from 0 to inf, until it returns finished as true.
	What it does: it goes through the string and returns the first N characters, but makes sure not to break any Rich Text tags */
	UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
	void GetFirstChars(FString originalString, bool isRichText, int32 chars, FString& outputString, bool& finished);
	
	//The actor this dialogue belongs to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( ExposeOnSpawn = true ), Category = "Dialogue UI")
	AActor* NPCActor;
};
