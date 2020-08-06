// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/FixLuaRuntime.h"
#include "UnLuaPrivate.h"
#include "PlatformFilemanager.h"
#include "IPlatformFilePak.h"

static TArray<FString> FixLuaName;
static TArray<FString> FixLuaPathName;
static TArray<FString> FixLuaPath;

int UFixLuaRuntime::GetAllFile(const FString & FilePath)
{
	if (FixLuaName.Num() > 0)
	{
		FixLuaName.Empty();
		FixLuaPathName.Empty();
		FixLuaPath.Empty();
	}

	FString SearchedFiles = FilePath;
	TArray<FString> FindedFiles;
	FPlatformFileManager::Get().GetPlatformFile().FindFilesRecursively(FindedFiles, *SearchedFiles, nullptr);
	for (auto i : FindedFiles)
	{
		FString FilePathRel;
		FString FileName;
		FString FileExtension;
		FPaths::Split(i, FilePathRel, FileName, FileExtension);

		FString left;
		FString right;
		FilePathRel.Split(FPaths::Combine(FilePath, TEXT("/")), &left, &right);

		if (right.Len() > 0)
			right.ReplaceInline(TEXT("/"), TEXT("."));

		if (right.Len() != 0)
		{
			right.Append(TEXT("."));
			right.Append(FileName);
			FixLuaPathName.Add(right);
		}
		else
		{
			FixLuaPathName.Add(FileName);
		}
		FixLuaName.Add(FileName);

		FixLuaPath.Add(i);
	}
	return FixLuaName.Num();
}

FString UFixLuaRuntime::GetFixLuaName(int i)
{
	return FixLuaName[i];
}

FString UFixLuaRuntime::GetFixLuaPathName(int i)
{
	return FixLuaPathName[i];
}

FString UFixLuaRuntime::GetFixPath(int i)
{
	return FixLuaPath[i];
}


FString UFixLuaRuntime::GetScriptPath()
{
	return FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Script"));
	//WrapperFile->Unmount()
}