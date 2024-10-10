// Fill out your copyright notice in the Description page of Project Settings.


#include "SEntryListWidget.h"
#include "Components/TextBlock.h"
#include "MyCustomStruct.h"

void USEntryListWidget::NativeConstruct()
{
    Super::NativeConstruct();
    UUListViewObject* ListItem = nullptr;
    NativeOnListItemObjectSet(ListItem);
}

void USEntryListWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    if (const UUListViewObject* ListItem = Cast<UUListViewObject>(ListItemObject))
    {
        DataText_1->SetText(FText::FromString(ListItem->MyCustomStruct.MyLevel.DisplayName));
        DataText_2->SetText(FText::AsNumber(ListItem->MyCustomStruct.MyLevel.ValueDouble));
    }
}
