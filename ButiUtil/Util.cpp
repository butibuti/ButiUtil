#include"stdafx.h"

#include "Util.h"
#include<random>
#include<direct.h>
#include"../../ButiMemorySystem/ButiMemorySystem/ButiPtr.h"
#include"ButiUtil/ButiUtil/Helper/StringHelper.h"
#include"ButiMemorySystem/ButiMemorySystem/ButiList.h"
#include"BinaryObject.h"
#include<filesystem>
#include<fstream>
#include<ctime>
#define WIN32_LEAN_AND_MEAN
#include<Windows.h>

void ButiEngine::Util::WStringtoMultiByte(const std::wstring & src, std::string & dest)
{
	size_t i;
	char *pMBstr = new char[src.length() * MB_CUR_MAX + 1];
	wcstombs_s(
		&i,
		pMBstr,
		src.length() * MB_CUR_MAX + 1,
		src.c_str(),
		_TRUNCATE	//すべて変換できなかったら切り捨て
	);
	dest = pMBstr;
	delete[] pMBstr;
}


std::string ButiEngine::Util::WStringToString(std::wstring oWString)

{
	// wstring → SJIS
	std::int32_t iBufferSize = WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str()
		, -1, (char *)NULL, 0, NULL, NULL);

	// バッファの取得
	CHAR* cpMultiByte = new CHAR[iBufferSize];

	// wstring → SJIS
	WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str(), -1, cpMultiByte
		, iBufferSize, NULL, NULL);

	// stringの生成
	std::string oRet(cpMultiByte, cpMultiByte + iBufferSize - 1);

	// バッファの破棄
	delete[] cpMultiByte;

	// 変換結果を返す
	return(oRet);
}

std::wstring ButiEngine::Util::StringToWString(std::string oString) {
	// SJIS → wstring
	std::int32_t iBufferSize = MultiByteToWideChar(CP_ACP, 0, oString.c_str()
		, -1, (wchar_t*)NULL, 0);

	// バッファの取得
	wchar_t* cpUCS2 = new wchar_t[iBufferSize];

	// SJIS → wstring
	MultiByteToWideChar(CP_ACP, 0, oString.c_str(), -1, cpUCS2
		, iBufferSize);

	// stringの生成
	std::wstring oRet(cpUCS2, cpUCS2 + iBufferSize - 1);

	// バッファの破棄
	delete[] cpUCS2;

	// 変換結果を返す
	return(oRet);
}

void ButiEngine::Util::MultiBytetoWString(const std::string& src, std::wstring& dest)
{
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

std::string ButiEngine::Util::ToUTF8(const std::string& arg_srcSjis)
{
	//Unicodeへ変換後の文字列長を得る
	std::int32_t lenghtUnicode = MultiByteToWideChar(CP_THREAD_ACP, 0, arg_srcSjis.c_str(), arg_srcSjis.size() + 1, NULL, 0);

	//必要な分だけUnicode文字列のバッファを確保
	wchar_t* bufUnicode = new wchar_t[lenghtUnicode];

	//ShiftJISからUnicodeへ変換
	MultiByteToWideChar(CP_THREAD_ACP, 0, arg_srcSjis.c_str(), arg_srcSjis.size() + 1, bufUnicode, lenghtUnicode);

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

std::string ButiEngine::Util::UTF8ToMultiByte(const std::string& arg_srcUTF8)
{
	return std::string();
}

void ButiEngine::Util::MakeDirectory(const std::string& arg_directoryName)
{
	if (!arg_directoryName.size()) { return; }
	if (_mkdir(arg_directoryName.c_str())) {
		MakeDirectory(StringHelper::RemoveLastDirectory(arg_directoryName));
		_mkdir(arg_directoryName.c_str());
	}
}

bool ButiEngine::Util::ExistFile(const std::string& arg_filePath) {
	std::ifstream checkedFile(arg_filePath);
	return checkedFile.is_open();
}

void ButiEngine::ThrowButiException_Runtime(const std::wstring & message1, const std::wstring & message2, const std::wstring & message3)
{//throw runtime_error function
	std::wstring outputMessage = message1;
	outputMessage += L"\n";
	outputMessage += message2;
	outputMessage += L"\n";
	outputMessage += message3;
	std::string output;

	Util::WStringtoMultiByte(outputMessage, output);
	throw std::runtime_error(output);
}

void ButiEngine::ThrowButiException_Runtime(const std::string & meesage1, const std::string & message2, const std::string & message3)
{//throw runtime_error function
	std::string outputMessage = meesage1;
	outputMessage += "\n";
	outputMessage += message2;
	outputMessage += "\n";
	outputMessage += message3;
	throw std::runtime_error(outputMessage);
}

ButiEngine::Value_ptr< std::random_device> g_vlp_rnd_device = nullptr;
ButiEngine::Value_ptr<std::mt19937>g_vlp_mt = nullptr;
ButiEngine::Value_ptr< std::uniform_int_distribution<>> g_vlp_randRange = nullptr;
ButiEngine::List<std::int32_t> g_list_randomLog;
std::string g_randomLogFileDir, g_randomLogFilePath;
std::int32_t g_logFileIndex=0,g_logIndex=0;
enum class RandomMode :std::int8_t {
	None,Log,Replay
};
RandomMode g_randomMode=RandomMode::None;
void ButiEngine::ButiRandom::Initialize()
{
	g_vlp_rnd_device = make_value<std::random_device>();
	g_vlp_mt = make_value<std::mt19937>((*g_vlp_rnd_device)());
	g_vlp_randRange = make_value< std::uniform_int_distribution<>>(0, 1);

}

void ButiEngine::ButiRandom::StartLog(const std::string& arg_logFileDir)
{
	if (g_randomMode == RandomMode::Log|| g_randomMode == RandomMode::Replay) { return; }
	g_randomLogFileDir = arg_logFileDir;
	g_list_randomLog.Clear();
	g_randomMode = RandomMode::Log;
	g_randomLogFilePath = g_randomLogFileDir + std::to_string(g_logFileIndex) + ".rlog";
}

void ButiEngine::ButiRandom::EndLog()
{
	g_randomMode = RandomMode::None;
}

void ButiEngine::ButiRandom::StartReplay(const std::string& arg_logFileDir)
{
	if (g_randomMode == RandomMode::Replay) { return; }
	g_randomLogFileDir = arg_logFileDir;
	g_list_randomLog.Clear();
	g_randomMode = RandomMode::Replay;
	g_logIndex = 0;
	std::stringstream stream;
	std::filesystem::directory_iterator itr(g_randomLogFileDir), end;
	std::error_code err;
	for (; itr != end && !err; itr.increment(err)) {
		ButiEngine::List<std::int32_t> list_data;
		if (StringHelper::GetExtension(itr->path().string()) != "rlog") { continue; }
		BinaryReader_File reader(itr->path().string());
		auto size = reader.ReadVariable<std::int32_t>();
		for (std::int32_t index = 0; index < size; index++) {
			list_data.Add(reader.ReadVariable<std::int32_t>());
		}
		g_list_randomLog.Add(list_data);
	}
}

void ButiEngine::ButiRandom::StartReplay(const std::int32_t* arg_p_Data, const std::int32_t arg_dataSize)
{
	if (g_randomMode == RandomMode::Replay) { return; }
	g_list_randomLog.Clear();
	for (std::int32_t index = 0; index < arg_dataSize; index++) {
		g_list_randomLog.Add( arg_p_Data[index]);
	}
	g_randomMode = RandomMode::Replay;
}

void ButiEngine::ButiRandom::EndReplay()
{
	g_randomMode = RandomMode::None;
}

void RandomLog(const std::int32_t arg_int) {
	g_list_randomLog.Add(arg_int);

	ButiEngine::Util::MakeDirectory(g_randomLogFileDir);
	ButiEngine::BinaryWriter_File writer(g_randomLogFilePath);
	writer.WriteVariable<std::int32_t>( g_list_randomLog.GetSize());
	for (auto log: g_list_randomLog) {
		writer.WriteVariable(log);
	}
	if (g_list_randomLog.GetSize() > 1024) {
		g_list_randomLog.Clear();
		g_logFileIndex++;
		g_randomLogFilePath = g_randomLogFileDir + std::to_string(g_logFileIndex) + ".rlog";
	}
}

std::int32_t ButiEngine::ButiRandom::GetInt(const std::int32_t arg_min, const std::int32_t arg_max)
{

	if (g_randomMode == RandomMode::Replay&& g_logIndex < g_list_randomLog.GetSize()) {
		auto output = g_list_randomLog.at(g_logIndex);
		g_logIndex++;
		return output;
	}
	if (arg_min == arg_max) {
		if (g_randomMode == RandomMode::Log) {
			RandomLog(arg_min);
		}
		return arg_min;
	}
	auto min = arg_min,max=arg_max;
	if (min > max) {
		auto b = min;
		min = max;
		max = min;
	}

	g_vlp_randRange = make_value< std::uniform_int_distribution<>>(min, max );
	auto output = (*g_vlp_randRange)(*g_vlp_mt);

	if (g_randomMode == RandomMode::Log) {
		RandomLog(output);
	}
	return output ;
}

timespec* ButiEngine::ButiTime::timespecSubstruction(const timespec* A, const timespec* B, timespec* C)
{
	C->tv_sec = A->tv_sec - B->tv_sec;
	C->tv_nsec = A->tv_nsec - B->tv_nsec;
	if (C->tv_nsec < 0) {
		C->tv_sec--;
		C->tv_nsec += 1000000000;
	}

	return C;
}
