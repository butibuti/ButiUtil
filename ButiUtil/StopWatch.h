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
	/// �o�ߎ��Ԃ��~���b�Ŏ擾
	/// </summary>
	/// <returns>�o�ߎ���</returns>
	std::uint64_t GetMillSecond()const;
	/// <summary>
	/// �o�ߎ��Ԃ�b�Ŏ擾
	/// </summary>
	/// <returns>�o�ߎ���</returns>
	std::uint64_t GetSecond()const;
private:
	class Impl;
	Impl* m_p_impl;
};

}

#endif // !STOPWATCH_H
