// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSVLoader.generated.h"

USTRUCT(BlueprintType)
struct FUserInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
		FString name;
	UPROPERTY(VisibleAnywhere)
		int32 age;
	UPROPERTY(VisibleAnywhere)
		bool gender;
	UPROPERTY(VisibleAnywhere)
		float height;
	UPROPERTY(VisibleAnywhere)
		FString mbti;
};

USTRUCT(BlueprintType)
struct FProductInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
		FString name;
	UPROPERTY(VisibleAnywhere)
		int32 price;
	UPROPERTY(VisibleAnywhere)
		bool sale;
	UPROPERTY(VisibleAnywhere)
		float weight;
};

UCLASS()
class CSVSTUDY_API ACSVLoader : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACSVLoader();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//void Parse(FString path);
	TArray<FUserInfo> Parse(FString path);

	template<typename T>
	TArray<T> Parse(FString path, UScriptStruct* pScriptStruct); //UScriptStruct = 스트럭트(구조체) 안에 있는 변수들을 접근한다.

	UPROPERTY(EditAnywhere)
	TArray<FUserInfo> userData;

	UPROPERTY(EditAnywhere)
		TArray<FProductInfo> productData;

};


