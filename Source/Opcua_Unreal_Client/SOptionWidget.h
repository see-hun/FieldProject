// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResolutionOption.h"
#include "SOptionWidget.generated.h"

/**
 * 
 */
UCLASS()
class OPCUA_UNREAL_CLIENT_API USOptionWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

protected:

	// �޺��ڽ��� �ػ� �ɼ��� �߰��ϴ� �Լ�
	void PopulateResolutionComboBox();

	UFUNCTION()
	void OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnClickedclose();

	UFUNCTION()
	void OnScreenModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType);


	UPROPERTY(meta = (BindWidget))
	class UButton* Button_close;

	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* ComboBox_Resolution;

	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* ComboBox_ScreenMode;

	// �ػ� ���
	TArray<FResolutionOption> ResolutionOptions;

	void ApplyResolution(FVector2D Resolution);

	void ApplyScreenMode(const FString& SelectedMode);
};
