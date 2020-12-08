// Fill out your copyright notice in the Description page of Project Settings.

#include "SRTReaderComponent.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

// Sets default values for this component's properties
USRTReaderComponent::USRTReaderComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	CurrentFilePath = TEXT("");
	CurrentTime = 0;
	CurrentSRTDataIndex = 0;
	CurrentSRTData = FSRTText();
	bIsPlaying = false;
}


// Called when the game starts
void USRTReaderComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USRTReaderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bIsPlaying)
	{
		CurrentTime += DeltaTime;
		if (CurrentSRTData.BeginTime<= CurrentTime && CurrentSRTData.EndTime >= CurrentTime)
		{
			CurrentText = CurrentSRTData.Text;
		}
		
		if (CurrentSRTData.EndTime < CurrentTime)
		{
			CurrentText = TEXT("");
			if (SRTDatas.Num() > CurrentSRTDataIndex + 1)
			{
				CurrentSRTDataIndex += 1;
				CurrentSRTData = SRTDatas[CurrentSRTDataIndex];
			}
			else
			{
				Stop();
			}
		}
		
	}
	// ...
}

bool USRTReaderComponent::LoadSRTFile(const FString& FilePath)
{
	bool Res = false;

	FString FileString;
	Res = FFileHelper::LoadFileToString(FileString, *(FPaths::ProjectContentDir() + FilePath));

	if (Res)
	{
		CurrentFilePath = FilePath;
		SRTDatas.Empty();
		TArray<FString> OutArray;
		FileString.ParseIntoArray(OutArray,TEXT("\r\n\r\n"));
		for (auto SectionString:OutArray)
		{
			FSRTText SRTText;
			TArray<FString> LineString;
			SectionString.ParseIntoArray(LineString, TEXT("\n"));
			if (LineString.Num()<3)
			{
				continue;
			}
			TArray<FString> TempStringArray;
			LineString[1] =LineString[1].Replace(TEXT(","), TEXT(":"));
			LineString[1] =LineString[1].Replace(TEXT(" "), TEXT(":"));
			LineString[1].ParseIntoArray(TempStringArray, TEXT(":"));
			if (TempStringArray.Num() != 9)
			{
				continue;
			}
			SRTText.BeginTime	= FCString::Atoi(*TempStringArray[0]) * 3600 + FCString::Atoi(*TempStringArray[1]) * 60 + FCString::Atoi(*TempStringArray[2]);
			SRTText.EndTime		= FCString::Atoi(*TempStringArray[5]) * 3600 + FCString::Atoi(*TempStringArray[6]) * 60 + FCString::Atoi(*TempStringArray[7]);
			SRTText.Text = TEXT("");
			for (int32 Index=2;Index< LineString.Num();++Index)
			{
				SRTText.Text.Append(LineString[Index]);
				SRTText.Text.Append(TEXT("\n"));
			}
			SRTDatas.Add(SRTText);
		}
	}
	return Res;
}

bool USRTReaderComponent::Play(const FString & FilePath)
{
	bool Res = CurrentFilePath == FilePath;
	if (!Res)
	{
		Res = LoadSRTFile(FilePath);
	}
	if (Res&&SRTDatas.Num()>0)
	{
		bIsPlaying = true;
		CurrentFilePath = FilePath;
		CurrentTime = 0;
		CurrentSRTDataIndex = 0;
		CurrentSRTData = SRTDatas[0];
	}
	return Res;
}

void USRTReaderComponent::Stop()
{
	CurrentText = TEXT("");
	bIsPlaying = false;
	CurrentTime = 0;
	CurrentSRTDataIndex = 0;
	CurrentSRTData = FSRTText();
	OnFinishEvent.Broadcast();
}

