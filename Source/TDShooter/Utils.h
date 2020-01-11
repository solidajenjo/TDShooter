#pragma once


#include "Engine.h"

#define LOG_INFO(t) { UE_LOG(LogTemp,Log, TEXT("%s"), *t);}
#define LOG_ERROR(t) { UE_LOG(LogTemp,Error, TEXT("%s"), *t);}
#define LOG_WARNING(t) { UE_LOG(LogTemp,Warning, TEXT("%s"), *t);}
#define LOG_SCREEN_DT(t, dt) {GEngine->AddOnScreenDebugMessage(-1, dt, FColor::Green, t);}


