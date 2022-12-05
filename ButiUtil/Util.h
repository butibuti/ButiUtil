#ifndef UTIL_H
#define UTIL_H
#include<string>
#include"Exception.h"


struct timespec;
namespace ButiEngine {
namespace BitFlag {
constexpr std::uint32_t FLAG_MAX = 16;
constexpr std::uint32_t BIT_FLAG[]{ (1 << 0),(1 << 1),(1 << 2),(1 << 3),(1 << 4),(1 << 5),(1 << 6),(1 << 7),(1 << 8),(1 << 9),(1 << 10),(1 << 11),(1 << 12),(1 << 13),(1 << 14),(1 << 15) };
}
class IApplication;
namespace Util
{
template<typename T>
class MemoryReleaser {
public:
	MemoryReleaser(T** arg_p_memoryAddress) :p_memoryAddress(arg_p_memoryAddress) {
		std::int32_t i = 0;
	}
	~MemoryReleaser()
	{
		if (*p_memoryAddress) {
			delete (*p_memoryAddress);
		}
	}
private:
	T** p_memoryAddress;
};

inline bool GetBitFlag(const std::int32_t arg_flag, const std::int32_t arg_bitIndex) {
	if (arg_bitIndex >= 16 || arg_bitIndex < 0) {
		return false;
	}
	return arg_flag & BitFlag::BIT_FLAG[arg_bitIndex];
}
inline std::int8_t GetBitNum(const std::int8_t arg_byte, const std::int32_t arg_readRange) {

	std::int8_t out = arg_byte;
	out <<= (8 - arg_readRange);
	out >>= (8 - arg_readRange);


	return std::abs(out);
}
inline std::int8_t GetBitNum(const std::int8_t arg_byte, const std::int32_t arg_readRange, const std::int32_t arg_offset) {

	std::int8_t out = arg_byte;
	out >>= (arg_offset);
	out <<= (8 - arg_readRange);
	out >>= (8 - arg_readRange);


	return std::abs(out);
}

void WStringtoMultiByte(const std::wstring& src, std::string& dest);
std::string WStringToString(std::wstring oWString);
std::wstring StringToWString(std::string oString);
void MultiBytetoWString(const std::string& src, std::wstring& dest);
std::string ToUTF8(const std::string& arg_srcSjis);
std::string UTF8ToMultiByte(const std::string& arg_srcUTF8);
void MakeDirectory(const std::string& arg_directoryName);
bool ExistFile(const std::string& arg_filePath);
void SetLogDirectory(const std::string& arg_logPath);
const std::string& GetLogDirectory();
void Log(const std::string& arg_logStr);

template<typename T>
static std::wstring GetWStringTypeName() {
	std::wstring outputClassName;
	MultiBytetoWString(typeid(T).name(), outputClassName);
	return outputClassName;
}
template<typename T>
static std::string GetStringTypeName() {
	return typeid(T).name();
}

static std::int32_t BitRemoveLeft(const std::int32_t arg_bit, const std::int32_t arg_slideSize) {

	std::int32_t ret = arg_bit;
	ret <<= arg_slideSize;
	ret >>= arg_slideSize;
	return ret;
}
static std::int32_t BitRemoveRight(const std::int32_t arg_bit, const std::int32_t arg_slideSize) {
	std::int32_t ret = arg_bit;
	ret >>= arg_slideSize;
	ret <<= arg_slideSize;
	return ret;
}
};

constexpr std::uint32_t LEVEL_FLAG[]{ (111 << 0),(111 << 3),(111 << 6),(111 << 9),(111 << 12),(111 << 15),(111 << 18),(111 << 21),(111 << 24),(111 << 27), };
class OctreeHelper {
public:
	static inline std::uint32_t BitSeparate(std::int8_t n)
	{
		std::uint32_t s = n;
		s = (s | s << 8) & 0x0000f00f;
		s = (s | s << 4) & 0x000c30c3;
		s = (s | s << 2) & 0x00249249;
		return s;
	}
	static inline std::int8_t GetLevel(const std::uint32_t& arg_flag, const std::uint8_t  arg_level) {
		std::int8_t out = 1;
		for (std::int8_t i = 0; i < arg_level; i++) {

			std::uint32_t Check = (arg_flag >> (i * 3)) & 0x7;
			if (Check != 0)
				out = i + 1;
		}
		return out;
	}
};
namespace ButiTime {
timespec* timespecSubstruction(const struct timespec* A, const struct timespec* B, struct timespec* C);

std::string GetYear_Month_Day_Hour_Min_Sec();
}

static void ThrowButiException_Runtime(const std::wstring& message1, const std::wstring& message2, const std::wstring& message3);
//throw runtime_error function
static void ThrowButiException_Runtime(const std::string& meesage1, const std::string& message2, const std::string& message3);



extern class ButiRandom {
public:

	static void Initialize();
	static void StartLog(const std::string& arg_logFileDir);
	static void EndLog();
	static void StartReplay(const std::string & arg_logFileDir);
	static void StartReplay(const std::int32_t* arg_p_Data, const std::int32_t arg_dataSize);
	static void EndReplay();
	static std::int32_t GetInt(const std::int32_t arg_min, const std::int32_t arg_max);

	template<class T>
	static T GetRandom(const T arg_min,const T arg_max,const std::int32_t arg_pase =1) {
		return static_cast<T> (GetInt(arg_min*arg_pase,arg_max*arg_pase)) / arg_pase;
	};

private:
	ButiRandom() {};
	~ButiRandom() {};
};

enum class SamplerState {
	LinearClamp,
	LinearWrap,
	SamplerNone,
	PointWrap,
	PointClamp,
	AnisotropicWrap,
	AnisotropicClamp,
	ComparisonLinear,
};
template<typename T>
void hash_combine(size_t& seed, T const& v) {

	std::hash<T> primitive_type_hash;


	seed ^= primitive_type_hash(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}
inline std::string U8(const std::string& arg_str) {
	return Util::ToUTF8(arg_str);
}
}

#endif
