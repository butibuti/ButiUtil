#include"stdafx.h"

#include "Util.h"
#include<random>



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
	int iBufferSize = WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str()
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
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, oString.c_str()
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

std::shared_ptr< std::random_device> shp_rnd_device = nullptr;
std::shared_ptr<std::mt19937>shp_mt = nullptr;
std::shared_ptr< std::uniform_int_distribution<>> shp_randRange = nullptr;

void ButiEngine::ButiRandom::Initialize()
{
	shp_rnd_device = std::make_shared<std::random_device>();
	shp_mt = std::make_shared<std::mt19937>((*shp_rnd_device)());
	shp_randRange = std::make_shared< std::uniform_int_distribution<>>(0, 1);

}


std::int32_t ButiEngine::ButiRandom::GetInt(const std::int32_t arg_min, const std::int32_t arg_max)
{

	if (arg_min == arg_max) {
		return arg_min;
	}
	auto min = arg_min,max=arg_max;
	if (min > max) {
		auto b = min;
		min = max;
		max = min;
	}

	shp_randRange = std::make_shared< std::uniform_int_distribution<>>(min, max );
	return (*shp_randRange)(*shp_mt) ;
}
