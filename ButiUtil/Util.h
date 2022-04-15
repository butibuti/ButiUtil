#ifndef UTIL_H
#define UTIL_H
#define WIN32_LEAN_AND_MEAN 
#pragma once
#include<string>
#include<ctime>
#include<memory>
#include<Windows.h>

#include<fstream>
#include"Exception.h"
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
static void MultiBytetoWString(const std::string& src, std::wstring& dest) {
	size_t i;
	wchar_t* WCstr = new wchar_t[src.length() + 1];
	mbstowcs_s(
		&i,
		WCstr,
		src.length() + 1,
		src.c_str(),
		_TRUNCATE //すべて変換できなかったら切り捨て
	);
	dest = WCstr;
	delete[] WCstr;
}

static std::string ToUTF8(const std::string& srcSjis) {
	//Unicodeへ変換後の文字列長を得る
	std::int32_t lenghtUnicode = MultiByteToWideChar(CP_THREAD_ACP, 0, srcSjis.c_str(), srcSjis.size() + 1, NULL, 0);

	//必要な分だけUnicode文字列のバッファを確保
	wchar_t* bufUnicode = new wchar_t[lenghtUnicode];

	//ShiftJISからUnicodeへ変換
	MultiByteToWideChar(CP_THREAD_ACP, 0, srcSjis.c_str(), srcSjis.size() + 1, bufUnicode, lenghtUnicode);

	//UTF8へ変換後の文字列長を得る
	std::int32_t lengthUTF8 = WideCharToMultiByte(CP_UTF8, 0, bufUnicode, -1, NULL, 0, NULL, NULL);

	//必要な分だけUTF8文字列のバッファを確保
	char* bufUTF8 = new char[lengthUTF8];

	//UnicodeからUTF8へ変換
	WideCharToMultiByte(CP_UTF8, 0, bufUnicode, lenghtUnicode + 1, bufUTF8, lengthUTF8, NULL, NULL);

	std::string strUTF8(bufUTF8);

	delete[]bufUnicode;
	delete[]bufUTF8;

	return strUTF8;
}
static std::string UTF8ToMultiByte(const std::string& srcUTF8) {
	//Unicodeへ変換後の文字列長を得る
	std::int32_t lenghtUnicode = MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, NULL, 0);

	//必要な分だけUnicode文字列のバッファを確保
	wchar_t* bufUnicode = new wchar_t[lenghtUnicode];

	//UTF8からUnicodeへ変換
	MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, bufUnicode, lenghtUnicode);

	//ShiftJISへ変換後の文字列長を得る
	std::int32_t lengthSJis = WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, -1, NULL, 0, NULL, NULL);

	//必要な分だけShiftJIS文字列のバッファを確保
	char* bufShiftJis = new char[lengthSJis];

	//UnicodeからShiftJISへ変換
	WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, lenghtUnicode + 1, bufShiftJis, lengthSJis, NULL, NULL);

	std::string strSJis(bufShiftJis);

	delete[]bufUnicode;
	delete[]bufShiftJis;

	return strSJis;
}

static bool ExistFile(const std::string& arg_filePath) {
	std::ifstream checkedFile(arg_filePath);

	return checkedFile.is_open();

}
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
static inline timespec* timespecSubstruction(const struct timespec* A, const struct timespec* B, struct timespec* C)
{
	C->tv_sec = A->tv_sec - B->tv_sec;
	C->tv_nsec = A->tv_nsec - B->tv_nsec;
	if (C->tv_nsec < 0) {
		C->tv_sec--;
		C->tv_nsec += 1000000000;
	}

	return C;
}
}

static void ThrowButiException_Runtime(const std::wstring& message1, const std::wstring& message2, const std::wstring& message3);
//throw runtime_error function
static void ThrowButiException_Runtime(const std::string& meesage1, const std::string& message2, const std::string& message3);



extern class ButiRandom {
public:

	static void Initialize();

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

///Cereal Regist
/// 
///

}

#endif
