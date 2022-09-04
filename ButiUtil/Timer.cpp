#include"stdafx.h"
#include"Timer.h"
using namespace::ButiEngine;

float Timer::st_globalSpeed = 1.0f;

Timer::Timer(float arg_maxCountframe)
{
	maxCountFrame = arg_maxCountframe;
	currentCountFrame = 0;
	isOn = false;
}

bool ButiEngine::Timer::IsOn() const
{
	return isOn;
}

bool& ButiEngine::Timer::IsOn() 
{
	return isOn;
}

void Timer::Start()
{
	isOn = true;
}

void Timer::Stop()
{
	isOn = false;
}

void Timer::Initialize()
{
	currentCountFrame = 0;
	isOn = false;
}

void ButiEngine::Timer::PreInitialize()
{
}

void ButiEngine::Timer::SetCount(std::int32_t arg_currentCount)
{
	currentCountFrame = arg_currentCount;
}

void ButiEngine::Timer::ChangeCountFrame(std::int32_t arg_maxCount)
{
	maxCountFrame = arg_maxCount;
}

void ButiEngine::Timer::Reset()
{
	currentCountFrame = 0;
}


