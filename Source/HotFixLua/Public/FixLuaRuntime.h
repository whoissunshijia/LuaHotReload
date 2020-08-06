// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FixLuaRuntime.generated.h"

/**
 * 
 */
UCLASS()
class HOTFIXLUA_API UFixLuaRuntime : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static int GetAllFile(const FString & FilePath);

	UFUNCTION(BlueprintCallable)
	static	FString GetFixLuaName(int i);

	UFUNCTION(BlueprintCallable)
	static	FString GetFixLuaPathName(int i);

	UFUNCTION(BlueprintCallable)
	static	FString GetFixPath(int i);

	UFUNCTION(BlueprintCallable)
	static	FString GetScriptPath();
};
