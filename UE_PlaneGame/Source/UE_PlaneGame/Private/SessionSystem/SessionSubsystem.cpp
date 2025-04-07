// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionSystem/SessionSubsystem.h"

#include "Debug.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Blueprint/UserWidget.h"
#include "Online/OnlineSessionNames.h"


USessionSubsystem::USessionSubsystem()
{
	//this->LoadingScreen = ConstructorHelpers::FClassFinder<UUserWidget>(TEXT("/Game/_DungeonCompanyContent/Code/UI/JoiningScreen")).Class;
}

void USessionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get();

	if (!SubSystem)
	{
		Debug::Print("Sub System is not valid");
		return;
	}
		

	SessionInterface = SubSystem->GetSessionInterface();

	if (!SessionInterface.IsValid())
	{
		Debug::Print("SessionInterface Is not valid");
		return;
	}
		

	SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &USessionSubsystem::OnCreateSessionComplete);
	SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &USessionSubsystem::OnFindSessionComplete);
	SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &USessionSubsystem::OnJoinSessionComplete);
	SessionInterface->OnSessionUserInviteAcceptedDelegates.AddUObject(
		this, &USessionSubsystem::OnSessionUserInviteAccepted);
}

void USessionSubsystem::OnSessionUserInviteAccepted(const bool bWasSuccessful, const int32 ControllerId,
                                                    FUniqueNetIdPtr UserId,
                                                    const FOnlineSessionSearchResult& InviteResult)
{
	
	if(IsValid(LoadingScreen))
		CreateWidget<UUserWidget>(GetWorld(),LoadingScreen)->AddToViewport(1);
	
	SessionInterface->JoinSession(0, NAME_GameSession, InviteResult);
}


void USessionSubsystem::CreateServer(FString ServerName, FString HostName, bool bIsPrivate, FString LevelPath)
{
	this->LevelPathToTravelTo=LevelPath;

	Debug::Print("Session Interface Name: "+IOnlineSubsystem::Get()->GetSubsystemName().ToString());
	Debug::Print("CreatingServer...");
	FOnlineSessionSettings sessionSettings;

	sessionSettings.bAllowJoinInProgress = true;
	sessionSettings.bIsDedicated = false;
	sessionSettings.bIsLANMatch = false;
	sessionSettings.bShouldAdvertise = true;
	sessionSettings.bAllowInvites=true;
	sessionSettings.bAllowJoinViaPresence=true;
	sessionSettings.bUsesPresence = true;
	sessionSettings.NumPublicConnections = 4;
	
	sessionSettings.bUseLobbiesIfAvailable = true;
	sessionSettings.Set(FName("SERVER_NAME_KEY"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	sessionSettings.Set(FName("SERVER_HOSTNAME_KEY"), HostName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	
	bool bsuccessful=SessionInterface->CreateSession(0, NAME_GameSession, sessionSettings);
}

void USessionSubsystem::OnCreateSessionComplete(FName SessionName, bool Succeeded)
{
	Debug::Print("OnCreateSessionComplete Succeeded: "+ LexToString(Succeeded));

	if (!Succeeded)
		return;

	//"/Game/_DungeonCompanyContent/Maps/MainDungeonBaked?listen"
	this->LevelPathToTravelTo+="?listen";
	GetWorld()->ServerTravel(this->LevelPathToTravelTo);
}

void USessionSubsystem::FindServers()
{
	UE_LOG(LogTemp, Warning, TEXT("Searching for Sessions..."));
	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	SessionSearch->bIsLanQuery = false;
	SessionSearch->MaxSearchResults = 100;//big number because of other steam users with the same appId
	SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void USessionSubsystem::OnFindSessionComplete(bool Succeeded)
{
	UE_LOG(LogTemp, Warning, TEXT("OnFindSessionComplete Succeeded: %d"), Succeeded);

	if (!Succeeded)
		return;

	TArray<FServerInfo> infos;
	int i = 0;

	for (FOnlineSessionSearchResult SR : SessionSearch->SearchResults)
	{
		if (!SR.IsValid())
			continue;

		if(SR.Session.SessionSettings.NumPublicConnections==0)
			continue;
		
		FServerInfo info;
		FString serverName = "Empty server same";
		FString hostName = "Empty host name";

		SR.Session.SessionSettings.Get(FName("SERVER_NAME_KEY"), serverName);
		SR.Session.SessionSettings.Get(FName("SERVER_HOSTNAME_KEY"), hostName);

		info.ServerName = serverName;
		info.MaxPlayers = SR.Session.SessionSettings.NumPublicConnections;
		info.CurrentPlayers = info.MaxPlayers - SR.Session.NumOpenPublicConnections;
		info.SetPlayerCount();
		info.ArrayIndex = i;
		++i;

		infos.Add(info);
	}

	SearchComplete.Broadcast(infos);
	//SearchComplete.Broadcast(infos);
}

void USessionSubsystem::JoinServer(int32 Index)
{
	FOnlineSessionSearchResult result = SessionSearch->SearchResults[Index];
	if (!result.IsValid())
	{
		Debug::Print("Joining session failed. Session is not Valid. Session index: "+Index);
		return;
	}
	Debug::Print("Joining session at index: "+  FString::FromInt(Index)+ "...");
	SessionInterface->JoinSession(0, NAME_GameSession, result);
}

void USessionSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{

	FString stringResult="";
	switch (Result)
	{
	case EOnJoinSessionCompleteResult::Success:
		stringResult = "Success";
		break;
	case EOnJoinSessionCompleteResult::SessionIsFull:
		stringResult = "SessionIsFull";
		break;
	case EOnJoinSessionCompleteResult::SessionDoesNotExist:
		stringResult = "SessionDoesNotExist";
		break;
	case EOnJoinSessionCompleteResult::CouldNotRetrieveAddress:
		stringResult = "CouldNotRetrieveAddress";
		break;
	case EOnJoinSessionCompleteResult::AlreadyInSession:
		stringResult = "AlreadyInSession";
		break;
	case EOnJoinSessionCompleteResult::UnknownError:
		stringResult = "UnknownError";
		break;
	default:
		stringResult = "InvalidResult";
		break;
	}

	Debug::Print("Join Session Result: "+ stringResult,10);
	if(Result!=EOnJoinSessionCompleteResult::Success)
	{
		Debug::Print("JoinSession failed.");
		return;
	}
		
	
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();

	if (!Controller)
		return;

	FString JoinAdress = "";
	
	if(SessionInterface->GetResolvedConnectString(SessionName, JoinAdress))
	{
		Debug::Print("GetResolved connect String sucsess");
	}
	else
	{
		Debug::Print("GetResolved connect String FAIL");
	}

	Debug::Print("JoinAdress: "+JoinAdress);

	JoinAdress = JoinAdress.Replace(*FString(":0"), *FString(":7777"), ESearchCase::IgnoreCase);

	if (JoinAdress == "")
		return;
	
	Debug::Print("Traveling to: %s"+JoinAdress);
	//UE_LOG(LogTemp, Warning, TEXT("Traveling to: %s"), *JoinAdress);
	Controller->ClientTravel(JoinAdress, ETravelType::TRAVEL_Absolute);
}

void USessionSubsystem::DestroyCurrentSession()
{
	if (!SessionInterface->GetNamedSession(NAME_GameSession))
		return;

	SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &USessionSubsystem::OnSessionLeft);
	SessionInterface->DestroySession(NAME_GameSession);
}

void USessionSubsystem::OnSessionLeft(FName SessionName, bool bWasSuccessful)
{
	if(!bWasSuccessful)
		return;
	
	GetWorld()->GetFirstPlayerController()->ClientTravel("/Game/_DungeonCompanyContent/Maps/MainMenu",TRAVEL_Absolute);
}
