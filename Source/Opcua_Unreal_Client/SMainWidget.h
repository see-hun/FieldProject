// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "UListViewObject.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyCustomStruct.h"
#include "SMainWidget.generated.h"
/**
 * 
 */
UCLASS()
class OPCUA_UNREAL_CLIENT_API USMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickedOff();

	UFUNCTION()
	void OnClickedOption();
	UFUNCTION()
	void OnResetButtonClicked();

	void UpdateDateTime();

	UPROPERTY(meta = (BindWidget))
	class UButton* home;

	UPROPERTY(meta = (BindWidget))
	class UWidget* BP_ShutDownWidget;

	UPROPERTY(meta = (BindWidget))
	class UWidget* BP_optionwidget;

	UPROPERTY(meta = (BindWidget))
	class UButton* Off;

	UPROPERTY(meta = (BindWidget))
	class UButton* option;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_Timer;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UListView* ListView;

	UFUNCTION(BlueprintCallable, Category = "DateTime")
	FText GetCurrentDateTime() const;

private:
	FTimerHandle TimerHandle;
};
