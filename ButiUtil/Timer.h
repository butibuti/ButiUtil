#ifndef TIMER_H
#define TIMER_H
#pragma once
#include <memory>
#include"ObjectFactory.h"

#ifndef CEREAL_NVP
#define CEREAL_NVP(T) ::cereal::make_nvp(#T, T)
#endif // !CEREAL_NVP

#ifndef ARCHIVE_BUTI
#define ARCHIVE_BUTI(v)\
	archive(CEREAL_NVP(v));
#endif // !ARCHIVE_BUTI

#ifndef ARCHIVE2_BUTI
#define ARCHIVE2_BUTI(v,v2)\
	archive(CEREAL_NVP(v),CEREAL_NVP(v2));
#endif // !ARCHIVE2_BUTI
#ifndef ARCHIVE3_BUTI
#define ARCHIVE3_BUTI(v,v2,v3)\
	archive(CEREAL_NVP(v),CEREAL_NVP(v2),CEREAL_NVP(v3));
#endif // !ARCHIVE3_BUTI
#ifndef ARCHIVE4_BUTI
#define ARCHIVE4_BUTI(v,v2,v3,v4)\
	archive(CEREAL_NVP(v),CEREAL_NVP(v2),CEREAL_NVP(v3),CEREAL_NVP(v4));
#endif // !ARCHIVE4_BUTI

namespace ButiEngine {

class Timer :public IObject
{
public:
	Timer(float arg_maxCountFrame);
	virtual bool Update() = 0;
	virtual bool Update_continue() = 0;
	bool IsOn()const;
	bool& IsOn();
	void Start();
	void Stop();
	void Initialize()override;
	void PreInitialize()override;
	void SetCount(std::int32_t arg_currentCount);
	void ChangeCountFrame(std::int32_t arg_maxCount);
	void Reset();
	inline float GetPercent() const {
		if (maxCountFrame == 0)return 0;
		return (float)currentCountFrame / (float)maxCountFrame;
	}
	inline float GetRemainFrame()const {
		return maxCountFrame - currentCountFrame;
	}
	float GetMaxCountFrame()const { return maxCountFrame; }
	float GetCurrentCountFrame()const { return currentCountFrame; }
	float& GetMaxCountFrame(){ return maxCountFrame; }
	float& GetCurrentCountFrame(){ return currentCountFrame; }
	static inline  float GetGlobalSpeed() { return st_globalSpeed; }
	static inline void SetGlobalSpeed(const float arg_speed) { st_globalSpeed = arg_speed; }
protected:
	float maxCountFrame;
	float currentCountFrame;
	bool isOn;
	static float st_globalSpeed;
};

class AbsoluteTimer :public Timer
{
public:
	AbsoluteTimer() :Timer(0) {}
	AbsoluteTimer(float arg_maxCountFrame) :Timer(arg_maxCountFrame) {}
	inline bool Update() override {
		if (!isOn) {
			return false;
		}
		currentCountFrame++;
		if (currentCountFrame >= maxCountFrame) {
			currentCountFrame = 0;
			return true;
		}
		return false;
	}
	inline bool Update_continue() override {
		if (!isOn) {
			return false;
		}
		currentCountFrame++;
		if (currentCountFrame >= maxCountFrame) {
			return true;
		}
		return false;
	}
	template<class Archive>
	void serialize(Archive& archive)
	{
		ARCHIVE_BUTI(currentCountFrame);
		ARCHIVE_BUTI(maxCountFrame);
		ARCHIVE_BUTI(isOn);
	}
};

class RelativeTimer :public Timer
{
public:
	RelativeTimer() :Timer(0) {}
	RelativeTimer(const float arg_maxCountFrame) :Timer(arg_maxCountFrame) {}
	inline bool Update()override {
		if (!isOn) {
			return false;
		}
		currentCountFrame += GetGlobalSpeed();
		if (currentCountFrame >= maxCountFrame) {
			currentCountFrame = 0;
			return true;
		}
		return false;
	}
	inline bool Update_continue() override {
		if (!isOn) {
			return false;
		}
		currentCountFrame += GetGlobalSpeed();
		if (currentCountFrame >= maxCountFrame) {
			return true;
		}
		return false;
	}

	Value_ptr<RelativeTimer> Clone() {
		auto cloned = ObjectFactory::Create<RelativeTimer>(maxCountFrame);

		cloned->isOn = isOn;

		return cloned;
	}

	template<class Archive>
	void serialize(Archive& archive)
	{
		ARCHIVE_BUTI(currentCountFrame);
		ARCHIVE_BUTI(maxCountFrame);
		ARCHIVE_BUTI(isOn);
	}
};

template <class T>
class TimeBomb :public Timer
{

public:
	using TsFunction = void (T::*)();
	TimeBomb(float arg_maxCountFrame, Value_ptr<T> arg_vlp_instance) :Timer(arg_maxCountFrame) {
		vlp_instance = arg_vlp_instance;
	}
	inline bool Update()override {
		if (!isOn) {
			return false;
		}
		currentCountFrame += GetGlobalSpeed();
		if (currentCountFrame >= maxCountFrame) {
			currentCountFrame = 0;
			(*(vlp_instance.lock()).*bombFunc)();
			return true;
		}
		return false;
	}
	inline bool Update_UnExecute() {
		if (!isOn) {
			return false;
		}
		currentCountFrame += GetGlobalSpeed();
		if (currentCountFrame >= maxCountFrame) {
			currentCountFrame = maxCountFrame;
			return false;
		}
		return false;
	}
	TsFunction bombFunc;

private:
	Value_weak_ptr<T> vlp_instance;
};
}


#endif // !TIMER_H
