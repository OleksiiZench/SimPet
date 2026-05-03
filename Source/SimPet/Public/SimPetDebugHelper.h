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
	
	static void Print(const FString &VectorTitle, const FVector &VectorValueToPrint, const FColor &Color = FColor::MakeRandomColor(), int32 InKey = -1)
	{
		if (GEngine)
		{
			const FString FinalMsg = VectorTitle + TEXT(": ") + VectorValueToPrint.ToString();

			GEngine->AddOnScreenDebugMessage(InKey, 7.0f, Color, FinalMsg);

			UE_LOG(LogTemp, Warning, TEXT("%s"), *FinalMsg);
		}
	}
	
	static void Print(const FString &RotatorTitle, const FRotator &RotatorValueToPrint, const FColor &Color = FColor::MakeRandomColor(), int32 InKey = -1)
	{
		if (GEngine)
		{
			const FString FinalMsg = RotatorTitle + TEXT(": ") + RotatorValueToPrint.ToString();

			GEngine->AddOnScreenDebugMessage(InKey, 7.0f, Color, FinalMsg);

			UE_LOG(LogTemp, Warning, TEXT("%s"), *FinalMsg);
		}
	}
	
	static void Print(const FString &TransformTitle, const FTransform &TransformValueToPrint, const FColor &Color = FColor::MakeRandomColor(), int32 InKey = -1)
	{
		if (GEngine)
		{
			const FString FinalMsg = FString::Printf(TEXT("%s:\n  Loc: %s\n  Rot: %s\n  Scl: %s"),
				*TransformTitle,
				*TransformValueToPrint.GetLocation().ToString(),
				*TransformValueToPrint.Rotator().ToString(),
				*TransformValueToPrint.GetScale3D().ToString());

			GEngine->AddOnScreenDebugMessage(InKey, 7.0f, Color, FinalMsg);

			UE_LOG(LogTemp, Warning, TEXT("%s"), *FinalMsg);
		}
	}
	
	static void PrintError(const FString &Msg, int32 InKey = -1)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(InKey, 7.0f, FColor::Red, TEXT("ERROR:") + Msg);

			UE_LOG(LogTemp, Error, TEXT("%s"), *Msg);
		}
	}

	static void PrintError(const FString &FloatTitle, float FloatValueToPrint, int32 InKey = -1)
	{
		if (GEngine)
		{
			const FString FinalMsg = TEXT("ERROR:") + FloatTitle + TEXT(": ") + FString::SanitizeFloat(FloatValueToPrint);

			GEngine->AddOnScreenDebugMessage(InKey, 7.0f, FColor::Red, FinalMsg);

			UE_LOG(LogTemp, Error, TEXT("%s"), *FinalMsg);
		}
	}
	
	static void PrintError(const FString &IntTitle, int32 IntValueToPrint, int32 InKey = -1)
	{
		if (GEngine)
		{
			const FString FinalMsg = TEXT("ERROR:") + IntTitle + TEXT(": ") + FString::FromInt(IntValueToPrint);

			GEngine->AddOnScreenDebugMessage(InKey, 7.0f, FColor::Red, FinalMsg);

			UE_LOG(LogTemp, Error, TEXT("%s"), *FinalMsg);
		}
	}
	
	static void PrintError(const FString &BoolTitle, bool BoolValueToPrint, int32 InKey = -1)
	{
		if (GEngine)
		{
			FString FinalMsg = TEXT("ERROR:") + BoolTitle + TEXT(": ");
			
			if (BoolValueToPrint)
			{
				FinalMsg = TEXT("ERROR:") + BoolTitle + TEXT(": true");
			}
			else
			{
				FinalMsg = TEXT("ERROR:") + BoolTitle + TEXT(": false");
			}

			GEngine->AddOnScreenDebugMessage(InKey, 7.0f, FColor::Red, FinalMsg);

			UE_LOG(LogTemp, Error, TEXT("%s"), *FinalMsg);
		}
	}
	
	static void PrintError(const FString &VectorTitle, const FVector &VectorValueToPrint, int32 InKey = -1)
	{
		if (GEngine)
		{
			const FString FinalMsg = TEXT("ERROR:") + VectorTitle + TEXT(": ") + VectorValueToPrint.ToString();

			GEngine->AddOnScreenDebugMessage(InKey, 7.0f, FColor::Red, FinalMsg);

			UE_LOG(LogTemp, Error, TEXT("%s"), *FinalMsg);
		}
	}
	
	static void PrintError(const FString &RotatorTitle, const FRotator &RotatorValueToPrint, int32 InKey = -1)
	{
		if (GEngine)
		{
			const FString FinalMsg = TEXT("ERROR:") + RotatorTitle + TEXT(": ") + RotatorValueToPrint.ToString();

			GEngine->AddOnScreenDebugMessage(InKey, 7.0f, FColor::Red, FinalMsg);

			UE_LOG(LogTemp, Error, TEXT("%s"), *FinalMsg);
		}
	}
	
	static void PrintError(const FString &TransformTitle, const FTransform &TransformValueToPrint, int32 InKey = -1)
	{
		if (GEngine)
		{
			const FString FinalMsg = FString::Printf(TEXT("ERROR: %s:\n  Loc: %s\n  Rot: %s\n  Scl: %s"),
				*TransformTitle,
				*TransformValueToPrint.GetLocation().ToString(),
				*TransformValueToPrint.Rotator().ToString(),
				*TransformValueToPrint.GetScale3D().ToString());

			GEngine->AddOnScreenDebugMessage(InKey, 7.0f, FColor::Red, FinalMsg);

			UE_LOG(LogTemp, Error, TEXT("%s"), *FinalMsg);
		}
	}
}
