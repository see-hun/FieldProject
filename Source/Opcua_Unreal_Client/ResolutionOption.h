// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResolutionOption.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FResolutionOption
{
    GENERATED_BODY()

    // �ػ� �ؽ�Ʈ (��: 1920x1080)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resolution")
    FString ResolutionLabel;

    // �ػ� �� (���� x ����)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resolution")
    FVector2D ResolutionValue;
};

USTRUCT(BlueprintType)
struct FResolutionCategory
{
    GENERATED_BODY()

    // �ػ� ī�װ� (��: 16:9)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resolution")
    FString CategoryLabel;

    // �� ī�װ� ���� �ػ� ���
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resolution")
    TArray<FResolutionOption> Resolutions;
};