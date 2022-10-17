#pragma once
#ifndef STRINGHELPER_H
#define STRINGHELPER_H


#include<string>
#include<vector>
extern class StringHelper
{
public:
	~StringHelper();
	static std::vector<std::string> Split(const std::string& arg_source, const std::string& arg_cuttingSouece);
	static std::string Cut(const std::string& arg_source, const std::string& frontCuttingSouece, const std::string& arg_backCuttingSouece, bool arg_isContaisCutSource = true);
	static std::string BackIdentifer(const std::string& arg_source,   const std::string& arg_findSouece);
	static std::string Remove(const std::string& arg_source, const std::string& arg_removeSouece);
	static std::string RemoveNull(const std::string& arg_source);
	static std::string Remove(const std::string& arg_source, const std::uint32_t arg_removeIndex,const std::uint32_t arg_removeRange);
	static std::string RemoveComment(const std::string& arg_source);
	static bool Contains(const std::string& arg_source, const std::string& arg_findSource);
	static bool Contains(const std::wstring& arg_source, const std::wstring& arg_findSource);
	static bool Contains(const std::string& arg_source, const char arg_findSource);
	static std::string Replace(const std::string& arg_source, const std::string& arg_before,const std::string& arg_after);
	static void WStringToSafetyConvert(std::wstring& arg_source);
	static std::string GetDirectory(const std::string& arg_source);
	static std::string RemoveFrontDirectory(const std::string& arg_source);
	static std::string RemoveLastDirectory(const std::string& arg_source);
	static std::string GetFileName(const std::string& arg_source, const bool arg_isContainExtension);
	static std::string RemoveExtension(const std::string& arg_source);
	static std::string GetExtension(const std::string& arg_source);
	static bool ExtensionCheck(const std::string& arg_source, const std::string& arg_extension);
	static std::int8_t WordToMonth(const std::string& arg_source);
private:
	StringHelper();
};

#endif // !STRINGHELPER_H