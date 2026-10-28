// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatMessageWidget.generated.h"

/**
 * Displays a single chat message (sender + text).
 * Widget layout is handled in a Blueprint subclass.
 */
UCLASS(abstract)
class CX_API UChatMessageWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Sets the message data and notifies Blueprint to update the display */
	void SetMessage(const FString& InSender, const FString& InMessage);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "ChatUI")
	FString Sender;

	UPROPERTY(BlueprintReadOnly, Category = "ChatUI")
	FString Message;

	/** Implemented in Blueprint to display the new message */
	UFUNCTION(BlueprintImplementableEvent, Category = "ChatUI")
	void OnMessageSet();
};
