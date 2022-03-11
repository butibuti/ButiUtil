#pragma once

#ifdef BUTIUTILDLL_EXPORTS
#define BUTIUTIL_API __declspec(dllexport)
#else
#define BUTIUTIL_API __declspec(dllimport)
#endif
#include<string>
#include<vector>
extern class StringHelper
{
public:
	BUTIUTIL_API ~StringHelper();
	BUTIUTIL_API static std::vector<std::string> Split(const std::string& source, const std::string& cuttingSouece);
	BUTIUTIL_API static std::string Cut(const std::string& source, const std::string& frontCuttingSouece, const std::string& backCuttingSouece, bool isContaisCutSource = true);
	BUTIUTIL_API static std::string BackIdentifer(const std::string& source,   const std::string& findSouece);
	BUTIUTIL_API static std::string Remove(const std::string& source, const std::string& removeSouece);
	BUTIUTIL_API static std::string Remove(const std::string& source, const unsigned int removeIndex,const unsigned int removeRange);
	BUTIUTIL_API static std::string RemoveComment(const std::string& source);
	BUTIUTIL_API static bool Contains(const std::string& source, const std::string& findSource);
	BUTIUTIL_API static bool Contains(const std::wstring& source, const std::wstring& findSource);
	BUTIUTIL_API static bool Contains(const std::string& source, const char findSource);
	BUTIUTIL_API static void WStringToSafetyConvert(std::wstring& source);
	BUTIUTIL_API static std::string GetDirectory(const std::string& source);
	BUTIUTIL_API static std::string GetFileName(const std::string& source, const bool isContainExtension);
	BUTIUTIL_API static std::string RemoveExtension(const std::string& source);
private:
	BUTIUTIL_API StringHelper();
};

