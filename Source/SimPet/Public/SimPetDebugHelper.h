// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

namespace Debug
{
	static void Print(const FString &Msg, const FColor &Color = FColor::MakeRandomColor(), int32 InKey = -1)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(InKey, 7.0f, Color, Msg);

			UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
		}
	}

	static void Print(const FString &FloatTitle, float FloatValueToPrint, const FColor &Color = FColor::MakeRandomColor(), int32 InKey = -1)
	{
		if (GEngine)
		{
			const FString FinalMsg = FloatTitle + TEXT(": ") + FString::SanitizeFloat(FloatValueToPrint);

			GEngine->AddOnScreenDebugMessage(InKey, 7.0f, Color, FinalMsg);

			UE_LOG(LogTemp, Warning, TEXT("%s"), *FinalMsg);
		}
	}
	
	static void Print(const FString &IntTitle, int32 IntValueToPrint, const FColor &Color = FColor::MakeRandomColor(), int32 InKey = -1)
	{
		if (GEngine)
		{
			const FString FinalMsg = IntTitle + TEXT(": ") + FString::FromInt(IntValueToPrint);

			GEngine->AddOnScreenDebugMessage(InKey, 7.0f, Color, FinalMsg);

			UE_LOG(LogTemp, Warning, TEXT("%s"), *FinalMsg);
		}
	}
	
	static void Print(const FString &BoolTitle, bool BoolValueToPrint, const FColor &Color = FColor::MakeRandomColor(), int32 InKey = -1)
	{
		if (GEngine)
		{
			FString FinalMsg = BoolTitle + TEXT(": ");
			
			if (BoolValueToPrint)
			{
				FinalMsg = BoolTitle + TEXT(": true");
			}
			else
			{
				FinalMsg = BoolTitle + TEXT(": false");
			}

			GEngine->AddOnScreenDebugMessage(InKey, 7.0f, Color, FinalMsg);

			UE_LOG(LogTemp, Warning, TEXT("%s"), *FinalMsg);
		}
	}
}
