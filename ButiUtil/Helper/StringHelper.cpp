#include"stdafx.h"
#include "StringHelper.h"

static const char backSlash = '\\';
StringHelper::StringHelper()
{
}


StringHelper::~StringHelper()
{
}

std::vector<std::string> StringHelper::Split(const std::string& arg_source, const std::string& cuttingSouece)
{
	auto output = std::vector<std::string>();
	std::int32_t first = 0;
	std::int32_t last = arg_source.find_first_of(cuttingSouece);
	if (last == std::string::npos) {
		output.push_back(arg_source);
		return output;
	}
	while (first < arg_source.size())
	{
		auto subString = arg_source.substr(first, last - first);
		output.push_back(subString);
		first = last + 1;
		last = arg_source.find_first_of(cuttingSouece, first);
		if (last == std::string::npos) {
			last = arg_source.size();
		}
	}
	return output;
}

std::string StringHelper::Cut(const std::string& arg_source, const std::string& frontCuttingSource, const std::string& backCuttingSouece, bool isContaisCutSource)
{
	auto pos = arg_source.find(frontCuttingSource);
	auto endPos = arg_source.find(backCuttingSouece, pos + frontCuttingSource.length());
	if (pos == std::string::npos || endPos == std::string::npos) {
		return "";
	}
	if (isContaisCutSource) {
		return arg_source.substr(pos, endPos - pos + backCuttingSouece.size());
	}
	else
	{
		return arg_source.substr(pos + frontCuttingSource.size(), endPos - pos - frontCuttingSource.size());
	}
}

std::string StringHelper::BackIdentifer(const std::string& arg_source, const std::string& findSouece)
{
	enum findProgress{findchar, initSpace,identiferchar,endspace };
	findProgress progress = findchar;
	std::int32_t spaceCount = 0;

	auto endPos = arg_source.find(findSouece);
	std::uint64_t pos=endPos;
	if ( endPos == std::string::npos) {
		return "";
	}
	while (progress != endspace||pos<0) {

		char search = arg_source[pos];
		if (search == ' ' || search == '\t' || search == '\n') {
			if (progress == findchar) {
				progress = initSpace;
			}
			else if (progress == identiferchar) {
				progress = endspace;
			}
		}
		else {
			if (progress == initSpace) {
				progress = identiferchar;
			}
		}

		pos--;
	}
	pos += 2;

	if (pos == std::string::npos || endPos == std::string::npos) {
		return "";
	}

	{
		return arg_source.substr(pos , endPos -pos-spaceCount);
	}
}


std::string StringHelper::Remove(const std::string& arg_source, const std::string& removeSouece)
{
	if (removeSouece == "") {
		return arg_source;
	}
	std::string output = arg_source;
	std::uint64_t pos = output.find(removeSouece);
	while (pos != std::string::npos)
	{
		output.erase(pos, removeSouece.size());
		pos = output.find(removeSouece);
	}
	return output;
}

std::string StringHelper::RemoveNull(const std::string& arg_source)
{
	std::string output = arg_source;
	std::uint64_t pos = output.find('\0');
	if (pos != std::string::npos)
	{
		output.erase(pos,arg_source.size()-pos);
	}
	return output;
}

std::string StringHelper::Remove(const std::string& arg_source, const std::uint32_t removeIndex, const std::uint32_t removeRange)
{
	std::string output = arg_source;
	output.erase(removeIndex, removeRange);
	return output;
}

std::string StringHelper::RemoveComment(const std::string& arg_source)
{
	std::string output = arg_source;
	auto commentStartPos = output.find("/*");
	auto commentEndPos = output.find("*/");
	while (commentStartPos != std::string::npos && commentEndPos != std::string::npos) {
		output.erase(commentStartPos, commentEndPos - commentStartPos + 2);
		commentStartPos = output.find("/*");
		commentEndPos = output.find("*/");
	}
	commentStartPos = output.find("//");
	while (commentStartPos != std::string::npos) {
		commentEndPos = output.find("\n", commentStartPos);
		if (commentEndPos != std::string::npos) {
			output.erase(commentStartPos, commentEndPos - commentStartPos);
		}
		else
		{
			output.erase(commentStartPos, output.size() - commentStartPos);
		}
		commentStartPos = output.find("//");
	}
	return output;
}

bool StringHelper::Contains(const std::string& arg_source, const std::string& findSource)
{
	if (arg_source.find(findSource) != std::string::npos)
		return true;
	else
		return false;
}

bool StringHelper::Contains(const std::wstring& arg_source, const std::wstring& findSource)
{
	if (arg_source.find(findSource) != std::string::npos)
		return true;
	else
		return false;
}

bool StringHelper::Contains(const std::string& arg_source, const char findSource)
{
	if (arg_source.find(findSource) != std::string::npos)
		return true;
	else
		return false;
}

std::string StringHelper::Replace(const std::string& arg_source, const std::string& arg_before, const std::string& arg_after)
{
	if (arg_before == "") {
		return arg_source;
	}
	std::string output = arg_source;
	std::uint64_t pos = output.find(arg_before);
	while (pos != std::string::npos)
	{
		output.erase(pos, arg_before.size());
		output.insert(pos, arg_after);
		pos = output.find(arg_before);
	}
	return output;
}


void StringHelper::WStringToSafetyConvert(std::wstring& arg_source)
{
	std::wstring backSlash = L"\\";
	auto backSlashPos = arg_source.find(backSlash);
	while (backSlashPos != std::wstring::npos)
	{
		arg_source.erase(backSlashPos, backSlash.size());
		arg_source.insert(backSlashPos, L"/");
		backSlashPos = arg_source.find(backSlash);
	}
}

std::string StringHelper::GetDirectory(const std::string& arg_source)
{
	if (!Contains(arg_source, "/")&&!Contains(arg_source,backSlash)) {
		return "";
	}

	if (arg_source[arg_source.size() - 1] == '/'|| arg_source[arg_source.size() - 1] == backSlash) {
		return arg_source;
	}
	auto source = arg_source;
	source = Replace(source, "\\", "/");
	auto splited = Split(source, "/");
	{
		std::vector<std::string> backSlashSplit;
		for (auto itr = splited.begin(), end = splited.end() ; itr != end; itr++) {

			auto sp = Split(*itr,std::string( &backSlash,1));
			backSlashSplit.insert(backSlashSplit.end(), sp.begin(), sp.end());
		}
		splited = backSlashSplit;
	}
	std::string out;

	if ( splited.begin() == (splited.end() - 1)) {
		return arg_source;
	}

	for (auto itr = splited.begin(),end= splited.end() - 1; itr != end; itr++) {
		out += (*itr) + "/";
	}

	

	return out;
}


std::string StringHelper::GetFileName(const std::string& arg_source, const bool isContainExtension)
{
	if ((!Contains(arg_source, "/") && !Contains(arg_source, backSlash))
		||(arg_source[arg_source.size()-1]=='/' )||( arg_source[arg_source.size() - 1] == '\\')) {
		return "";
	}
	auto source = Replace(arg_source, "\\", "/");
	auto splited = Split(source, "/");
	{
		std::vector<std::string> backSlashSplit;
		for (auto itr = splited.begin(), end = splited.end(); itr != end; itr++) {
			auto sp = Split(*itr,std::string( &backSlash,1));
			backSlashSplit.insert(backSlashSplit.end(), sp.begin(), sp.end());
		}
		splited = backSlashSplit;
	}

	std::string out = *splited.rbegin();

	if (isContainExtension)
		return out;
	if (!Contains(out, ".")) {
		return out;
	}
	out = Split(out, ".").at(0);

	return out;
}

std::string StringHelper::RemoveFrontDirectory(const std::string& arg_source)
{
	auto dir = GetDirectory(arg_source);
	auto splitedDir = Split(dir, "/"); dir.clear();
	for (std::int32_t index = 1; index < splitedDir.size(); index++) {
		dir += splitedDir[index]+"/";
	}
	return dir+GetFileName(arg_source,true);
}
std::string StringHelper::RemoveLastDirectory(const std::string& arg_source)
{
	auto dir = GetDirectory(arg_source);
	auto splitedDir = Split(dir, "/"); dir.clear();
	for (std::int32_t index = 0; index < splitedDir.size()-1; index++) {
		dir += splitedDir[index] + "/";
	}
	return dir + GetFileName(arg_source, true);
}
std::string StringHelper::RemoveExtension(const std::string& arg_source)
{
	auto splited = Split(arg_source, ".");
	if (splited.size() == 1) { return splited[0]; }
	std::string output;
	for (std::int32_t index = 0, size = splited.size(); index < size - 1;index++) {
		output +=index!=size-2? splited[index]+".": splited[index];
	}
	return output;
}
std::string StringHelper::GetExtension(const std::string& arg_source)
{
	auto splited = Split(arg_source, ".");
	return splited.at(splited.size() - 1);
}

bool StringHelper::ExtensionCheck(const std::string& arg_source, const std::string& arg_extension)
{
	auto splited = Split(arg_source, ".");
	return splited.at(splited.size() - 1)==arg_extension;
}

std::int8_t StringHelper::WordToMonth(const std::string& arg_source)
{
	if (arg_source == "Jan") {
		return 1;
	}
	if (arg_source == "Feb") {
		return 2;
	}
	if (arg_source == "Mar") {
		return 3;
	}
	if (arg_source == "Apr") {
		return 4;
	}
	if (arg_source == "May") {
		return 5;
	}
	if (arg_source == "Jun") {
		return 6;
	}
	if (arg_source == "Jul") {
		return 7;
	}
	if (arg_source == "Aug") {
		return 8;
	}
	if (arg_source == "Sep") {
		return 9;
	}
	if (arg_source == "Oct") {
		return 10;
	}
	if (arg_source == "Nov") {
		return 11;
	}
	if (arg_source == "Dec") {
		return 12;
	}
	return -1;
}

std::string StringHelper::GetLastDirectory(const std::string& arg_source)
{
	auto dir = GetDirectory(arg_source);
	auto splitedDir = Split(dir, "/");
    return splitedDir.size()? splitedDir[splitedDir.size()-1] : "";
}

