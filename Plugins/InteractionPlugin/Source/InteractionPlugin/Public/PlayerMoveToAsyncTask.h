// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Navigation/PathFollowingComponent.h"
#include "AITypes.h"
#include "PlayerMoveToAsyncTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerMoveToAsyncTaskOutputPin, FHitResult, ClickResult);

/**
 * 
 */
UCLASS()
class INTERACTIONPLUGIN_API UPlayerMoveToAsyncTask : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "AI|Navigation")
		static UPlayerMoveToAsyncTask* PlayerMoveTo(class AController* Controller, const FHitResult& HitResult);

public:
	UPROPERTY(BlueprintAssignable)
		FPlayerMoveToAsyncTaskOutputPin Success;

	UPROPERTY(BlueprintAssignable)
		FPlayerMoveToAsyncTaskOutputPin Failure;

protected:
	UPROPERTY()
		AController* Controller;

	UPROPERTY()
		FHitResult HitResult;

	UPROPERTY()
		FAIRequestID RequestID;

	virtual void Activate() override;

	//UFUNCTION()
		void OnMoveComplete(FAIRequestID FinishedRequestID, const FPathFollowingResult& Result);
};
