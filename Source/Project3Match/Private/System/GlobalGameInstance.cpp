// Fill out your copyright notice in the Description page of Project Settings.

#include "System/GlobalGameInstance.h"
#include "MISC/Match3BPFunctionLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "AchievementBlueprintLibrary.h"
#include "AchievementWriteCallbackProxy.h"

bool UGlobalGameInstance::CheckAchievements(int32 StIndex, int32 EdIndex, int32 StatToCheck, const bool& bForceUpdate)
{
    APlayerController* PC = Cast<APlayerController>(UMatch3BPFunctionLibrary::GetLocalPlayerController(this));
    for (int i = StIndex; i <= EdIndex; ++i)
    {
        float Percent = FMath::Clamp<float>((StatToCheck / (float)Achievements.AchievementValues[i]) * 100.f, 0, 100);
        if (bForceUpdate)
        {
            bool bFoundedID = false;
            float Progress = 0;
            UAchievementBlueprintLibrary::GetCachedAchievementProgress(this, PC, Achievements.AchievementNames[i], bFoundedID, Progress);
            if (bFoundedID)
            {
                if (Percent > Progress)
                    WriteAchievement(Achievements.AchievementNames[i], Percent, i);
                else
                {
                    Achievements.AchievementProgress[i] = Progress;
                    Achievements.CachedAchievementProgress[i] = Progress;
                }
            }
            else
            {
                if(Achievements.AchievementProgress[i] > Achievements.CachedAchievementProgress[i])
                    WriteAchievement(Achievements.AchievementNames[i], Percent, i);
            }
        }
        else
        {
            if (Percent > Achievements.AchievementProgress[i])
                Achievements.AchievementProgress[i] = Percent;

            if (Achievements.AchievementProgress[i] > Achievements.CachedAchievementProgress[i])
                WriteAchievement(Achievements.AchievementNames[i], Percent, i);
        }
    }
    return true;
}

float UGlobalGameInstance::ChooseLocalOrServerAchievementValues(int32 Index, float RemoteValue)
{
    return 0.0f;
}

/*static auto fn = UObject::FindFunction("Function OnlineSubsystemUtils.AchievementWriteCallbackProxy.WriteAchievementProgress");*/
