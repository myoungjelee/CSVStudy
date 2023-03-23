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
	//���� �����͸� ���� Array
	TArray<FUserInfo> data;

	//path �� �ִ� ������ �о����
	FString fileContent;
	if (FFileHelper::LoadFileToString(fileContent, *path))
	{
		//UE_LOG(LogTemp, Error, TEXT("%s"), *fileContent);
		
		//��(����) ������
		TArray<FString> rows;
		fileContent.ParseIntoArrayLines(rows);  //Enter �������� �������� �߶� �־��ְڴ�.

		//�����̸� ������
		TArray<FString> variableName;
		rows[0].ParseIntoArray(variableName, TEXT(","));

		//��(Data) ������
		for (int32 i = 1; i < rows.Num(); i++)
		{
			TArray<FString> values;
			rows[i].ParseIntoArray(values, TEXT(","));

			//�ϳ��� �����͸� ����ü�� ����
			FUserInfo info;
			info.name = values[0];
			info.age = FCString::Atoi(*values[1]); //��Ʈ�� ���� ��Ʈ�� ����
			info.gender = values[2].ToBool(); //Ʈ������,������ ���� �������� ��ȯ����
			info.height = FCString::Atof(*values[3]); //��Ʈ�� ���� �÷�Ʈ�� ����
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
	//���� �����͸� ���� Array
	TArray<T> data;

	//path �� �ִ� ������ �о����
	FString fileContent;
	if (FFileHelper::LoadFileToString(fileContent, *path))
	{
		//UE_LOG(LogTemp, Error, TEXT("%s"), *fileContent);

		//��(����) ������
		TArray<FString> rows;
		fileContent.ParseIntoArrayLines(rows);  //Enter �������� �������� �߶� �־��ְڴ�.

		//�����̸� ������
		TArray<FString> variableName;
		rows[0].ParseIntoArray(variableName, TEXT(","));

		//��(Data) ������
		for (int32 i = 1; i < rows.Num(); i++)
		{
			TArray<FString> values;
			rows[i].ParseIntoArray(values, TEXT(","));

			//�ϳ��� �����͸� ����ü�� ����
			T info;
			for (int32 j = 0; j < variableName.Num(); j++)
			{
				FProperty* pro = pScriptStruct->FindPropertyByName(*variableName[j]); //pScriptStruct�� �ش� ���� �̸����� �Ǿ��ִ� ������ ���� ������ pro �� ����

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
