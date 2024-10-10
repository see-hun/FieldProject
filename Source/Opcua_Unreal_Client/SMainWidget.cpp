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
        // ���� ���ü� ���¸� �����ͼ� ��� (���̸� �����, ����� ���̵���)
        ESlateVisibility CurrentVisibility = BP_ShutDownWidget->GetVisibility();

        if (CurrentVisibility == ESlateVisibility::Visible)
        {
            // ���ü��� ���̴� ���¶�� �����
            BP_ShutDownWidget->SetVisibility(ESlateVisibility::Hidden);
        }
        else
        {
            // ������ ���¶�� ���̰� �ϱ�
            BP_ShutDownWidget->SetVisibility(ESlateVisibility::Visible);
        }

        UE_LOG(LogTemp, Warning, TEXT("Button Clicked, Visibility Toggled"));
    }
}

void USMainWidget::OnClickedOption()
{
    if (BP_optionwidget)
    {
        // ���� ���ü� ���¸� �����ͼ� ��� (���̸� �����, ����� ���̵���)
        ESlateVisibility CurrentVisibility = BP_optionwidget->GetVisibility();

        if (CurrentVisibility == ESlateVisibility::Visible)
        {
            // ���ü��� ���̴� ���¶�� �����
            BP_optionwidget->SetVisibility(ESlateVisibility::Hidden);
        }
        else
        {
            // ������ ���¶�� ���̰� �ϱ�
            BP_optionwidget->SetVisibility(ESlateVisibility::Visible);
        }

        UE_LOG(LogTemp, Warning, TEXT("Button Clicked, Visibility Toggled"));
    }
}

//Ȩ��ư ������ �÷��̾� ��ŸƮ �������� �̵�
void USMainWidget::OnResetButtonClicked()
{

    // �÷��̾� ��Ʈ�ѷ����� ���� �� ��������
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
        // ���� �ð��� �����ͼ� �ؽ�Ʈ ��Ͽ� ����
        TextBlock_Timer->SetText(GetCurrentDateTime());
    }

}

FText USMainWidget::GetCurrentDateTime() const
{
    FDateTime Now = FDateTime::Now();

    // ��¥�� �ð��� ���ڿ��� ��ȯ
    FString DateTimeString = Now.ToString(TEXT("%Y-%m-%d %H:%M:%S"));

    // �ؽ�Ʈ�� ��ȯ �� ��ȯ
    return FText::FromString(DateTimeString);
}