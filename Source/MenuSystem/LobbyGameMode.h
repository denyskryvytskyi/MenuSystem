// Copyright Denys Kryvytskyi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "LobbyGameMode.generated.h"

/**
 *
 */
UCLASS()
class MENUSYSTEM_API ALobbyGameMode : public AGameModeBase {
    GENERATED_BODY()

public:
    void PostLogin(APlayerController* NewPlayer) override;
    void Logout(AController* Exiting) override;
};
