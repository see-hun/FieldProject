// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "MyCustomStruct.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UListViewObject.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)  // 블루프린트에서 사용할 수 있도록 표시
class OPCUA_UNREAL_CLIENT_API UUListViewObject : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "List")
    FMyCustomStruct MyCustomStruct;
};