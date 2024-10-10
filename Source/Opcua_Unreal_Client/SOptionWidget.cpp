// Fill out your copyright notice in the Description page of Project Settings.


#include "SOptionWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/GameUserSettings.h"
#include "Components/ComboBoxString.h"

void USOptionWidget::NativeConstruct()
{
    Super::NativeConstruct();

    PopulateResolutionComboBox();

    Button_close->OnClicked.AddDynamic(this, &USOptionWidget::OnClickedclose);

    // �޺��ڽ����� ������ ����� ���� ��������Ʈ ����
    ComboBox_Resolution->OnSelectionChanged.AddDynamic(this, &USOptionWidget::OnResolutionChanged);

    if (ComboBox_ScreenMode)
    {
        ComboBox_ScreenMode->AddOption(TEXT("Fullscreen"));
        ComboBox_ScreenMode->AddOption(TEXT("Windowed"));
        ComboBox_ScreenMode->AddOption(TEXT("Windowed Fullscreen"));

        // �޺��ڽ� ���� ����� �� ȣ��� �Լ��� ���ε�
        ComboBox_ScreenMode->OnSelectionChanged.AddDynamic(this, &USOptionWidget::OnScreenModeChanged);

        // �ʱ� ���ð� ���� (��: ���� ������ ȭ�� ��忡 ���缭)
        UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
        if (UserSettings)
        {
            switch (UserSettings->GetFullscreenMode())
            {
            case EWindowMode::Fullscreen:
                ComboBox_ScreenMode->SetSelectedOption(TEXT("Fullscreen"));
                break;
            case EWindowMode::Windowed:
                ComboBox_ScreenMode->SetSelectedOption(TEXT("Windowed"));
                break;
            case EWindowMode::WindowedFullscreen:
                ComboBox_ScreenMode->SetSelectedOption(TEXT("Windowed Fullscreen"));
                break;
            }
        }
    }
}

void USOptionWidget::PopulateResolutionComboBox()
{
    // ���� �ػ� �ɼ��� �߰�

    FResolutionOption Res1;
    Res1.ResolutionLabel = TEXT("1920x1080");
    Res1.ResolutionValue = FVector2D(1920, 1080);
    ResolutionOptions.Add(Res1);

    FResolutionOption Res2;
    Res2.ResolutionLabel = TEXT("1280x720");
    Res2.ResolutionValue = FVector2D(1280, 720);
    ResolutionOptions.Add(Res2);

    FResolutionOption Res3;
    Res3.ResolutionLabel = TEXT("800x600");
    Res3.ResolutionValue = FVector2D(800, 600);
    ResolutionOptions.Add(Res3);

    // �޺��ڽ��� �ػ� �߰�
    if (ComboBox_Resolution)
    {
        for (const FResolutionOption& ResOption : ResolutionOptions)
        {
            ComboBox_Resolution->AddOption(ResOption.ResolutionLabel);
        }
    }
}

void USOptionWidget::OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    // ���õ� �ػ󵵿� �´� ResolutionOption�� ã��
    for (const FResolutionOption& ResOption : ResolutionOptions)
    {
        if (ResOption.ResolutionLabel == SelectedItem)
        {
            // ���õ� �ػ� ����
            ApplyResolution(ResOption.ResolutionValue);
            break;
        }
    }
}

void USOptionWidget::ApplyResolution(FVector2D Resolution)
{
    // UGameUserSettings�� ���� �ػ� ���� ����
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    if (UserSettings)
    {
        // �ػ� ����
        UserSettings->SetScreenResolution(FIntPoint(Resolution.X, Resolution.Y));

        // ���� ���� ����
        UserSettings->ApplySettings(false);
    }

}

void USOptionWidget::ApplyScreenMode(const FString& SelectedMode)
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
    if (UserSettings)
    {
        if (SelectedMode == "Fullscreen")
        {
            UserSettings->SetFullscreenMode(EWindowMode::Fullscreen);
        }
        else if (SelectedMode == "Windowed")
        {
            UserSettings->SetFullscreenMode(EWindowMode::Windowed);
        }
        else if (SelectedMode == "Windowed Fullscreen")
        {
            UserSettings->SetFullscreenMode(EWindowMode::WindowedFullscreen);
        }

        // ���� ����
        UserSettings->ApplySettings(false);
    }
}

void USOptionWidget::OnClickedclose()
{
    SetVisibility(ESlateVisibility::Hidden);
}
void USOptionWidget::OnScreenModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    ApplyScreenMode(SelectedItem);
};