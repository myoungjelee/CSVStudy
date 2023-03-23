// Fill out your copyright notice in the Description page of Project Settings.


#include "CSVLoader.h"

// Sets default values
ACSVLoader::ACSVLoader()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACSVLoader::BeginPlay()
{
	Super::BeginPlay();

	//FString path = FPaths::Combine(FPaths::ProjectDir(), TEXT("CSVData/UserData.csv"));
	FString path = FString::Printf(TEXT("%s/CSVData/UserData.csv"), *FPaths::ProjectDir());
	FString path2 = FString::Printf(TEXT("%s/CSVData/ProductData.csv"), *FPaths::ProjectDir());

	//userData = Parse(path);
	userData = Parse<FUserInfo>(path, FUserInfo::StaticStruct());
	productData = Parse<FProductInfo>(path2, FProductInfo::StaticStruct());
}

// Called every frame
void ACSVLoader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<FUserInfo> ACSVLoader::Parse(FString path)
{
	//최종 데이터를 담을 Array
	TArray<FUserInfo> data;

	//path 에 있는 파일을 읽어오자
	FString fileContent;
	if (FFileHelper::LoadFileToString(fileContent, *path))
	{
		//UE_LOG(LogTemp, Error, TEXT("%s"), *fileContent);
		
		//행(한줄) 나누기
		TArray<FString> rows;
		fileContent.ParseIntoArrayLines(rows);  //Enter 기준으로 한줄한줄 잘라서 넣어주겠다.

		//변수이름 나누기
		TArray<FString> variableName;
		rows[0].ParseIntoArray(variableName, TEXT(","));

		//값(Data) 나누기
		for (int32 i = 1; i < rows.Num(); i++)
		{
			TArray<FString> values;
			rows[i].ParseIntoArray(values, TEXT(","));

			//하나의 데이터를 구조체에 넣자
			FUserInfo info;
			info.name = values[0];
			info.age = FCString::Atoi(*values[1]); //스트링 값을 인트로 변경
			info.gender = values[2].ToBool(); //트루폴스,예스노 값을 불형으로 반환해줌
			info.height = FCString::Atof(*values[3]); //스트링 값을 플로트로 변경
			info.mbti = values[4];

			data.Add(info);

// 			UE_LOG(LogTemp, Error, TEXT("-------------"));
// 			for (int32 j = 0; j < values.Num(); j++)
// 			{
// 				UE_LOG(LogTemp, Warning, TEXT("%s"), *values[j]);
// 			}
		}
	}
	return data;
}


template<typename T>
TArray<T> ACSVLoader::Parse(FString path, UScriptStruct* pScriptStruct)
{
	//최종 데이터를 담을 Array
	TArray<T> data;

	//path 에 있는 파일을 읽어오자
	FString fileContent;
	if (FFileHelper::LoadFileToString(fileContent, *path))
	{
		//UE_LOG(LogTemp, Error, TEXT("%s"), *fileContent);

		//행(한줄) 나누기
		TArray<FString> rows;
		fileContent.ParseIntoArrayLines(rows);  //Enter 기준으로 한줄한줄 잘라서 넣어주겠다.

		//변수이름 나누기
		TArray<FString> variableName;
		rows[0].ParseIntoArray(variableName, TEXT(","));

		//값(Data) 나누기
		for (int32 i = 1; i < rows.Num(); i++)
		{
			TArray<FString> values;
			rows[i].ParseIntoArray(values, TEXT(","));

			//하나의 데이터를 구조체에 넣자
			T info;
			for (int32 j = 0; j < variableName.Num(); j++)
			{
				FProperty* pro = pScriptStruct->FindPropertyByName(*variableName[j]); //pScriptStruct에 해당 변수 이름으로 되어있는 변수에 대한 정보가 pro 에 담긴다

				if (pro->IsA<FStrProperty>())
				{
					FString* valuePtr = pro->ContainerPtrToValuePtr<FString>(&info);
					*valuePtr = values[j];
				}
				else if(pro->IsA<FIntProperty>())
				{
					int32* valuePtr = pro->ContainerPtrToValuePtr<int32>(&info);
					*valuePtr = FCString::Atoi(*values[j]);
				}
				else if (pro->IsA<FBoolProperty>())
				{
					bool* valuePtr = pro->ContainerPtrToValuePtr<bool>(&info);
					*valuePtr = values[j].ToBool();
				}
				else if (pro->IsA<FFloatProperty>())
				{
					float* valuePtr = pro->ContainerPtrToValuePtr<float>(&info);
					*valuePtr = FCString::Atof(*values[j]);
				}
			}

			data.Add(info);

			// 			UE_LOG(LogTemp, Error, TEXT("-------------"));
			// 			for (int32 j = 0; j < values.Num(); j++)
			// 			{
			// 				UE_LOG(LogTemp, Warning, TEXT("%s"), *values[j]);
			// 			}
		}
	}

	return data;
}
