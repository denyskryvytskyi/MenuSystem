// Copyright Denys Kryvytskyi. All Rights Reserved.

#include "LobbyGameMode.h"

#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    if (GameState) {
        const int32 NumberOfPlayers = GameState->PlayerArray.Num();

        if (GEngine) {
            GEngine->AddOnScreenDebugMessage(1, 60.0f, FColor::Yellow, FString::Printf(TEXT("Players in the game: %d"), NumberOfPlayers));

            if (APlayerState* PlayerState = NewPlayer->GetPlayerState<APlayerState>()) {
                const FString PlayerName = PlayerState->GetPlayerName();
                GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Green, FString::Printf(TEXT("%s has joined the game"), *PlayerName));
            }
        }
    }
}

void ALobbyGameMode::Logout(AController* Exiting)
{
    Super::Logout(Exiting);

    if (GameState) {
        const int32 NumberOfPlayers = GameState->PlayerArray.Num();

        if (APlayerState* PlayerState = Exiting->GetPlayerState<APlayerState>()) {
            const FString PlayerName = PlayerState->GetPlayerName();

            if (GEngine) {
                GEngine->AddOnScreenDebugMessage(1, 60.0f, FColor::Yellow, FString::Printf(TEXT("Players in the game: %d"), NumberOfPlayers - 1));
                GEngine->AddOnScreenDebugMessage(2, 60.0f, FColor::Cyan, FString::Printf(TEXT("%s has exited the game"), *PlayerName));
            }
        }
    }
}
