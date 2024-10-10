// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include <common.h>
#include "MyCustomStruct.h"
#include "MyPlayerController.generated.h"

UCLASS()
class OPCUA_UNREAL_CLIENT_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

	AMyPlayerController();

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	UA_Client* MyClient;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> WidgetClass;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UListView* ListView;

	UFUNCTION(BlueprintCallable, Category = "ListView")
	void PopulateListView();

protected:
	virtual void SetupInputComponent() override;

private:
	void ConnectToOpcUaServer();
	void ReadMyLevelDataFromOpcUa();
	void SetFullscreenMode();
};
