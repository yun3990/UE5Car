// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/ChatMessageWidget.h"

void UChatMessageWidget::SetMessage(const FString& InSender, const FString& InMessage)
{
	Sender = InSender;
	Message = InMessage;
	OnMessageSet();
}
