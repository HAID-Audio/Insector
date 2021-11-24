// Fill out your copyright notice in the Description page of Project Settings.


#include "InsectorGameInstance.h"
#include "InsectorState.h"

UInsectorState* UInsectorGameInstance::GetInsectorState(TSubclassOf<UInsectorState> StateClass)
{
    for (UInsectorState* InsectorState : GameState)
    {
        if (InsectorState && InsectorState->GetClass() == StateClass)
        {
            return InsectorState;
        }
    }

    UInsectorState* NewInsectorState = NewObject<UInsectorState>(this, StateClass);

    GameState.Add(NewInsectorState);

    return NewInsectorState;
}
