#ifndef STOPWATCH_H
#define STOPWATCH_H
namespace ButiEngine {
class StopWatch
{
public:
	StopWatch();
	~StopWatch();
	void Start();
	void Stop();
	/// <summary>
	/// 経過時間をミリ秒で取得
	/// </summary>
	/// <returns>経過時間</returns>
	std::uint64_t GetMillSecond()const;
	/// <summary>
	/// 経過時間を秒で取得
	/// </summary>
	/// <returns>経過時間</returns>
	std::uint64_t GetSecond()const;
private:
	class Impl;
	Impl* m_p_impl;
};

}

#endif // !STOPWATCH_H
