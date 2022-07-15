#include"stdafx.h"
#include"ButiUtil/ButiUtil/StopWatch.h"
#include<chrono>

namespace ButiEngine {
class StopWatch::Impl {
public:
	std::timespec befTs, afterTs;
};
}

ButiEngine::StopWatch::StopWatch()
{
	m_p_impl = new Impl();
}
ButiEngine::StopWatch::~StopWatch()
{
	delete m_p_impl;
}
void ButiEngine::StopWatch::Start()
{
	std::timespec_get(&m_p_impl->befTs, TIME_UTC);
}
void ButiEngine::StopWatch::Stop()
{
	std::timespec_get(&m_p_impl->afterTs, TIME_UTC);
}
std::uint64_t ButiEngine::StopWatch::GetMillSecond() const
{
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::nanoseconds{ m_p_impl->afterTs.tv_nsec }) - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::nanoseconds{ m_p_impl->befTs.tv_nsec });
	return ms.count()+ (m_p_impl->afterTs.tv_sec - m_p_impl->befTs.tv_sec) * 1000;
}
std::uint64_t ButiEngine::StopWatch::GetSecond() const
{
	return (m_p_impl->afterTs.tv_sec - m_p_impl->befTs.tv_sec);
}