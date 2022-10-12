// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorProperties.h"
#include "Kismet/KismetInternationalizationLibrary.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "Kismet2/BlueprintEditorUtils.h"

#if WITH_EDITOR
UEditorProperties::UEditorProperties(const FObjectInitializer& obj)
{

	ShortcutKey = EKeys::L;
	GetLanguage.Add(English, "en");
	GetLanguage.Add(Chinese, "zh-hans");
	GetLanguage.Add(Japanese, "ja");
	GetLanguage.Add(Portuguese, "pt");
	GetLanguage.Add(Spanish, "es");
	GetLanguage.Add(Korean, "ko");
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	inputp = MakeShareable(new FInputProcessor(this));
	FSlateApplication::Get().RegisterInputPreProcessor(inputp);
}

UEditorProperties::~UEditorProperties()
{
	if (inputp.Get())
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(inputp);
	}
}

void FInputProcessor::Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> ICursor)
{

}

bool FInputProcessor::HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent)
{
	if (IsValid(Owner) && InKeyEvent.GetKey() == Owner->ShortcutKey.GetFName())
	{
		Owner->GetOpenTheBlueprint();
		FString Language = UKismetInternationalizationLibrary::GetCurrentLanguage();
		Owner->Switch = *Owner->GetLanguage.Find(Owner->SetLanguage);

		if (Language != Owner->Switch)
		{
			Owner->Previous = UKismetInternationalizationLibrary::GetCurrentLanguage();
			UKismetInternationalizationLibrary::SetCurrentLanguage(Owner->Switch);
		}
		else
		{
			UKismetInternationalizationLibrary::SetCurrentLanguage(Owner->Previous);
		}
		Owner->RefreshBlueprints();
	}
	return false;
}

void UEditorProperties::GetOpenTheBlueprint()
{

	UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
	EditedAssets = AssetEditorSubsystem->GetAllEditedAssets();

}

void UEditorProperties::RefreshBlueprints()
{
	if (EditedAssets.Num() > 0)
	{
		for (UObject* Data : EditedAssets)
		{
			TWeakObjectPtr<UBlueprint> Blueprint = Cast<UBlueprint>(Data);
			FBlueprintEditorUtils::RefreshAllNodes(Blueprint.Get());
		}

	}
}

bool FInputProcessor::HandleKeyUpEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent)
{

	return false;
}

bool FInputProcessor::HandleAnalogInputEvent(FSlateApplication& SlateApp, const FAnalogInputEvent& InAnalogInputEvent)
{

	return false;
}

bool FInputProcessor::HandleMouseMoveEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{

	return false;
}
#endif

