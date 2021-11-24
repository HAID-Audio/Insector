// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "InsectorGameInstance.generated.h"

class UInsectorState;

/**
 * 
 */
UCLASS()
class INSECTORTD_API UInsectorGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced, Category = "Insector Game Instance")
		TArray<UInsectorState*> GameState;

	UFUNCTION(BlueprintPure, Category = "Insector Game Instance", meta = (DeterminesOutputType = "StateClass"))
		UInsectorState* GetInsectorState(TSubclassOf<UInsectorState> StateClass);
};
