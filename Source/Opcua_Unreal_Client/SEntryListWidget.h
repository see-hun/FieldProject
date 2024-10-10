// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "UListViewObject.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "SEntryListWidget.generated.h"
/**
 * 
 */
UCLASS()
class OPCUA_UNREAL_CLIENT_API USEntryListWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;

    virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* DataText_1;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* DataText_2;
};
