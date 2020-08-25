// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

namespace UserEnum::Platform
{
	UENUM(BlueprintType)
		enum class EPlatformType : uint8
	{
		WINDOWS, IOS, ANDROID, HTML5, LINUX, OSX
	};
}