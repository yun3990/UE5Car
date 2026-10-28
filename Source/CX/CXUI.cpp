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
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/TextBlock.h"
#include "GameState/CXGameStateBase.h"
#include "PlayerState/CXPlayerState.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

void UCXUI::NativeConstruct()
{
    Super::NativeConstruct();
    if (!MatchStatusText && WidgetTree)
    {
        MatchStatusText = WidgetTree->ConstructWidget<UTextBlock>(
            UTextBlock::StaticClass(), TEXT("MatchStatusText"));
        MatchStatusText->SetJustification(ETextJustify::Center);
        MatchStatusText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
        MatchStatusText->SetShadowColorAndOpacity(FLinearColor::Black);
        MatchStatusText->SetShadowOffset(FVector2D(1.f, 1.f));
        FSlateFontInfo Font = MatchStatusText->GetFont();
        Font.Size = 24;
        MatchStatusText->SetFont(Font);
        MatchStatusText->SetVisibility(ESlateVisibility::HitTestInvisible);

        if (UCanvasPanel* Canvas = Cast<UCanvasPanel>(WidgetTree->RootWidget))
        {
            UCanvasPanelSlot* StatusSlot = Canvas->AddChildToCanvas(MatchStatusText);
            StatusSlot->SetAnchors(FAnchors(0.5f, 0.f));
            StatusSlot->SetAlignment(FVector2D(0.5f, 0.f));
            StatusSlot->SetPosition(FVector2D(0.f, 24.f));
            StatusSlot->SetAutoSize(true);
            StatusSlot->SetZOrder(10);
        }
        else
        {
            UWidget* PreviousRoot = WidgetTree->RootWidget;
            UOverlay* Overlay = WidgetTree->ConstructWidget<UOverlay>();
            WidgetTree->RootWidget = Overlay;
            if (PreviousRoot)
            {
                UOverlaySlot* ContentSlot = Overlay->AddChildToOverlay(PreviousRoot);
                ContentSlot->SetHorizontalAlignment(HAlign_Fill);
                ContentSlot->SetVerticalAlignment(VAlign_Fill);
            }
            UOverlaySlot* StatusSlot = Overlay->AddChildToOverlay(MatchStatusText);
            StatusSlot->SetHorizontalAlignment(HAlign_Center);
            StatusSlot->SetVerticalAlignment(VAlign_Top);
            StatusSlot->SetPadding(FMargin(0.f, 24.f, 0.f, 0.f));
        }
    }
    UpdateMatchStatus();
}

void UCXUI::UpdateMatchStatus()
{
    if (!MatchStatusText || !GetWorld())
    {
        return;
    }

    APlayerController* Owner = GetOwningPlayer();
    ACXPlayerState* LocalState = Owner ? Owner->GetPlayerState<ACXPlayerState>() : nullptr;
    if (UTextBlock* PlayerInfo = Cast<UTextBlock>(GetWidgetFromName(TEXT("PlayerStateInfo"))))
    {
        PlayerInfo->SetText(LocalState
            ? FText::FromString(LocalState->GetPlayerStateInfo())
            : FText::GetEmpty());
    }

    ACXGameState* State = GetWorld()->GetGameState<ACXGameState>();
    if (!State)
    {
        MatchStatusText->SetText(FText::FromString(TEXT("게임 정보를 기다리는 중")));
        return;
    }

    FString Label;
    switch (State->GetCurrentGameState())
    {
    case EGameState::Wating:
        Label = FString::Printf(TEXT("대기 중 | %d초"), State->GetRemainingTime());
        break;
    case EGameState::Playing:
        Label = FString::Printf(TEXT("게임 진행 중 | %d초"), State->GetRemainingTime());
        break;
    case EGameState::Finished:
        if (!LocalState || State->Winners.IsEmpty())
        {
            Label = TEXT("게임 종료 | 결과 확인 중");
        }
        else if (State->IsWinner(LocalState))
        {
            Label = State->Winners.Num() > 1 ? TEXT("게임 종료 | 공동 승리!") : TEXT("게임 종료 | 승리!");
        }
        else
        {
            Label = TEXT("게임 종료 | 패배");
        }
        break;
    }
    MatchStatusText->SetText(FText::FromString(Label));
}