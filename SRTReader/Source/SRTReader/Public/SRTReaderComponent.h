// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SRTReaderComponent.generated.h"


USTRUCT(BlueprintType)
struct FSRTText
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BeginTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EndTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Text;

	FSRTText()
	{
		BeginTime = 0;
		EndTime = 0;
		Text = TEXT("");
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SRTREADER_API USRTReaderComponent : public UActorComponent
{
	GENERATED_BODY()


public:	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishEvent);

	// Sets default values for this component's properties
	USRTReaderComponent();

	UPROPERTY(BlueprintAssignable, Category = "SRTReader")
	FOnFinishEvent OnFinishEvent;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "SRTReader")
	bool LoadSRTFile(const FString& FilePath);

	UFUNCTION(BlueprintCallable, Category = "SRTReader")
	bool Play(const FString& FilePath);

	UFUNCTION(BlueprintCallable, Category = "SRTReader")
	void Stop();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CurrentText;

	TArray<FSRTText> SRTDatas;

	float CurrentTime;
	int32 CurrentSRTDataIndex;
	FSRTText CurrentSRTData;
	FString CurrentFilePath;
	bool bIsPlaying;
};
