// Fill out your copyright notice in the Description page of Project Settings.


#include "SMainWidget.h"
#include "Components/Button.h"
#include "Components/Widget.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"
#include "TimerManager.h"
#include "Misc/DateTime.h"
#include "MyCustomStruct.h"
#include "MyPlayerController.h"
#include "Components/ListView.h"
#include "UListViewObject.h"
#include "SEntryListWidget.h"
#include "Blueprint/UserWidget.h"
#include "FindInBlueprints.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "FieldCameraPawn.h"



void USMainWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    BP_ShutDownWidget->SetVisibility(ESlateVisibility::Hidden);
    BP_optionwidget->SetVisibility(ESlateVisibility::Hidden);


    Off->OnClicked.AddDynamic(this, &USMainWidget::OnClickedOff);
    option->OnClicked.AddDynamic(this, &USMainWidget::OnClickedOption);
    home->OnClicked.AddDynamic(this, &USMainWidget::OnResetButtonClicked);

    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USMainWidget::UpdateDateTime, 1.0f, true);

    UpdateDateTime();

}

void USMainWidget::OnClickedOff()
{
    if (BP_ShutDownWidget)
    {
        // 현재 가시성 상태를 가져와서 토글 (보이면 숨기고, 숨기면 보이도록)
        ESlateVisibility CurrentVisibility = BP_ShutDownWidget->GetVisibility();

        if (CurrentVisibility == ESlateVisibility::Visible)
        {
            // 가시성이 보이는 상태라면 숨기기
            BP_ShutDownWidget->SetVisibility(ESlateVisibility::Hidden);
        }
        else
        {
            // 숨겨진 상태라면 보이게 하기
            BP_ShutDownWidget->SetVisibility(ESlateVisibility::Visible);
        }

        UE_LOG(LogTemp, Warning, TEXT("Button Clicked, Visibility Toggled"));
    }
}

void USMainWidget::OnClickedOption()
{
    if (BP_optionwidget)
    {
        // 현재 가시성 상태를 가져와서 토글 (보이면 숨기고, 숨기면 보이도록)
        ESlateVisibility CurrentVisibility = BP_optionwidget->GetVisibility();

        if (CurrentVisibility == ESlateVisibility::Visible)
        {
            // 가시성이 보이는 상태라면 숨기기
            BP_optionwidget->SetVisibility(ESlateVisibility::Hidden);
        }
        else
        {
            // 숨겨진 상태라면 보이게 하기
            BP_optionwidget->SetVisibility(ESlateVisibility::Visible);
        }

        UE_LOG(LogTemp, Warning, TEXT("Button Clicked, Visibility Toggled"));
    }
}

//홈버튼 누르면 플레이어 스타트 지점으로 이동
void USMainWidget::OnResetButtonClicked()
{

    // 플레이어 컨트롤러에서 현재 폰 가져오기
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn)
    {
        AFieldCameraPawn* FieldCameraPawn = Cast<AFieldCameraPawn>(PlayerPawn);
        if (FieldCameraPawn)
        {
            FieldCameraPawn->PowerOff();
            FieldCameraPawn->GoPlayerStartposition();
        }
     
    }
}

void USMainWidget::UpdateDateTime()
{
    if (TextBlock_Timer)
    {
        // 현재 시간을 가져와서 텍스트 블록에 설정
        TextBlock_Timer->SetText(GetCurrentDateTime());
    }

}

FText USMainWidget::GetCurrentDateTime() const
{
    FDateTime Now = FDateTime::Now();

    // 날짜와 시간을 문자열로 변환
    FString DateTimeString = Now.ToString(TEXT("%Y-%m-%d %H:%M:%S"));

    // 텍스트로 변환 후 반환
    return FText::FromString(DateTimeString);
}