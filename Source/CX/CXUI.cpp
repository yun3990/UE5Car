// Copyright Epic Games, Inc. All Rights Reserved.


#include "CXUI.h"
#include "UI/ChatUI.h"

void UCXUI::OnNewChatMessage(const FString& InSender, const FString& InMessage)
{
	UI_Chat->OnNewChatMessage(InSender, InMessage);
}

void UCXUI::UpdateSpeed(float NewSpeed)
{
	// format the speed to KPH or MPH
	float FormattedSpeed = FMath::Abs(NewSpeed) * (bIsMPH ? 0.022f : 0.036f);

	// call the Blueprint handler
	OnSpeedUpdate(FormattedSpeed);
}

void UCXUI::UpdateGear(int32 NewGear)
{
	// call the Blueprint handler
	OnGearUpdate(NewGear);
}
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "GameState/CXGameStateBase.h"
#include "PlayerState/CXPlayerState.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

void UCXUI::NativeConstruct()
{
    Super::NativeConstruct();
    UpdateMatchStatus();
}

void UCXUI::UpdateMatchStatus()
{
    if (!GetWorld())
    {
        return;
    }

    APlayerController* Owner = GetOwningPlayer();
    ACXPlayerState* LocalState = Owner ? Owner->GetPlayerState<ACXPlayerState>() : nullptr;
    ACXGameState* State = GetWorld()->GetGameState<ACXGameState>();
    FString Label = LocalState ? LocalState->GetPlayerStateInfo() : FString();

    if (State && State->GetCurrentGameState() == EGameState::Finished)
    {
        if (!LocalState || State->Winners.IsEmpty())
        {
            Label = TEXT("게임 종료 | 결과 확인 중");
        }
        else if (State->IsWinner(LocalState))
        {
            Label = State->Winners.Num() > 1 ? TEXT("공동 승리!") : TEXT("승리!");
        }
        else
        {
            Label = TEXT("패배");
        }
    }

    UWidget* PlayerInfo = GetWidgetFromName(TEXT("PlayerStateInfo"));
    const FText DisplayText = FText::FromString(Label);
    if (UTextBlock* Text = Cast<UTextBlock>(PlayerInfo))
    {
        Text->SetText(DisplayText);
    }
    else if (UEditableTextBox* TextBox = Cast<UEditableTextBox>(PlayerInfo))
    {
        TextBox->SetIsReadOnly(true);
        TextBox->SetText(DisplayText);
    }
}