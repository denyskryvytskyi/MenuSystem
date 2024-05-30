// Copyright Denys Kryvytskyi. All Rights Reserved.

#include "MenuWidget.h"

#include "MultiplayerSessionsSubsystem.h"

#include "MatchType.h"

#include "Components/Button.h"

constexpr int32 kMaxSearchSessionsResults = 10000;

void UMenuWidget::MenuSetup(const int32 numPublicConnections, const EMatchType matchType)
{
    NumPublicConnections = numPublicConnections;
    MatchType = matchType;

    AddToViewport();
    SetVisibility(ESlateVisibility::Visible);
    SetIsFocusable(true);

    if (UWorld* World = GetWorld()) {
        if (APlayerController* PlayerController = World->GetFirstPlayerController()) {
            FInputModeUIOnly InputModeData;
            InputModeData.SetWidgetToFocus(TakeWidget());
            InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

            PlayerController->SetInputMode(InputModeData);
            PlayerController->SetShowMouseCursor(true);
        }
    }

    if (UGameInstance* GameInstance = GetGameInstance()) {
        MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();

        if (MultiplayerSessionsSubsystem) {
            MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
            MultiplayerSessionsSubsystem->MultiplayerOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);
            MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);
            MultiplayerSessionsSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
            MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
        }
    }
}

bool UMenuWidget::Initialize()
{
    if (!Super::Initialize()) {
        return false;
    }

    if (HostButton) {
        HostButton->OnClicked.AddDynamic(this, &ThisClass::OnHostButtonClicked);
    }

    if (JoinButton) {
        JoinButton->OnClicked.AddDynamic(this, &ThisClass::OnJoinButtonClicked);
    }

    return true;
}

void UMenuWidget::NativeDestruct()
{
    RemoveFromParent();
    if (UWorld* World = GetWorld()) {
        if (APlayerController* PlayerController = World->GetFirstPlayerController()) {
            FInputModeGameOnly InputModeData;
            PlayerController->SetInputMode(InputModeData);
            PlayerController->SetShowMouseCursor(false);
        }
    }

    Super::NativeDestruct();
}

void UMenuWidget::OnCreateSession(bool bWasSuccessful)
{
    if (GEngine) {
        GEngine->AddOnScreenDebugMessage(-1,
                                         15.0f,
                                         FColor::Green,
                                         FString("UMenuWidget::OnCreateSession"));
    }

    if (bWasSuccessful) {
        UWorld* World = GetWorld();
        if (World) {
            World->ServerTravel(FString("/Game/ThirdPerson/Maps/Lobby?listen"));
        }
    }
}

void UMenuWidget::OnStartSession(bool bWasSuccessful)
{
}

void UMenuWidget::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
    if (MultiplayerSessionsSubsystem == nullptr) {
        return;
    }

    for (const auto& Result : SessionResults) {
        FString Id = Result.GetSessionIdStr();
        FString User = Result.Session.OwningUserName;
        int32 MatchTypeIndex;
        Result.Session.SessionSettings.Get(FName("MatchType"), MatchTypeIndex);
        const EMatchType MatchTypeResult = static_cast<EMatchType>(MatchTypeIndex);

        if (GEngine) {
            GEngine->AddOnScreenDebugMessage(
                -1,
                15.f,
                FColor::Green,
                FString::Printf(TEXT("Id: %s, User: %s"), *Id, *User));
        }

        if (MatchTypeResult == MatchType) {
            if (GEngine) {
                GEngine->AddOnScreenDebugMessage(
                    -1,
                    15.f,
                    FColor::Green,
                    FString::Printf(TEXT("Joining Match Type: %s"), *MatchTypeToString(MatchTypeResult)));
            }

            MultiplayerSessionsSubsystem->JoinSession(Result);
            break;
        }
    }
}

void UMenuWidget::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
    if (IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get()) {
        auto OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();

        if (!OnlineSessionInterface.IsValid()) {
            return;
        }

        FString Address;
        if (OnlineSessionInterface->GetResolvedConnectString(NAME_GameSession, Address)) {
            if (GEngine) {
                GEngine->AddOnScreenDebugMessage(
                    -1,
                    15.f,
                    FColor::Yellow,
                    FString::Printf(TEXT("Socket: %s"), *Address));
            }

            if (APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController()) {
                PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
            }
        }
    }
}

void UMenuWidget::OnDestroySession(bool bWasSuccessful)
{
}

void UMenuWidget::OnHostButtonClicked()
{
    if (MultiplayerSessionsSubsystem) {
        MultiplayerSessionsSubsystem->CreateSession(NumPublicConnections, MatchType);
    }
}

void UMenuWidget::OnJoinButtonClicked()
{
    if (MultiplayerSessionsSubsystem) {
        MultiplayerSessionsSubsystem->FindSessions(kMaxSearchSessionsResults);
    }
}
