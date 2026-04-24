// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "GameModes/SimPetMainLevelGameMode.h"

#include "GameFramework/GameUserSettings.h"

#include "Characters/SimPetPlayer.h"

ASimPetMainLevelGameMode::ASimPetMainLevelGameMode()
{
	DefaultPawnClass = ASimPetPlayer::StaticClass();
}

void ASimPetMainLevelGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	SetLowSettings();
}

void ASimPetMainLevelGameMode::SetLowSettings()
{
	UGameUserSettings *UserSettings = GEngine->GetGameUserSettings();
	
	if (UserSettings)
	{
		UserSettings->SetViewDistanceQuality(1);
		UserSettings->SetAntiAliasingQuality(1);
		UserSettings->SetPostProcessingQuality(0);
		UserSettings->SetShadowQuality(0);
		UserSettings->SetGlobalIlluminationQuality(0);
		UserSettings->SetReflectionQuality(1);
		UserSettings->SetTextureQuality(1);
		UserSettings->SetVisualEffectQuality(1);
		UserSettings->SetFoliageQuality(1);
		UserSettings->SetShadingQuality(1);
		
		UserSettings->ApplySettings(true);
	}
}
