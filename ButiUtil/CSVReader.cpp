#include"stdafx.h"
#include"CSVData.h"
#include"Helper/StringHelper.h"

ButiEngine::CSVReader::CSVReader()
{
}

ButiEngine::CSVReader::~CSVReader()
{
}

ButiEngine::Value_ptr< ButiEngine::CSVData>  ButiEngine::CSVReader::GetMatrix(const std::string& filePath)
{
	std::ifstream ifs(filePath);
	std::string source = std::string(std::istreambuf_iterator<char>(ifs),
		std::istreambuf_iterator<char>());

	auto splitedSource = StringHelper::Split(source, "\n");

	auto spS= StringHelper::Split(splitedSource.at(0), ",");

	auto output =make_value< CSVData>(spS.size(),splitedSource.size());

	for (std::uint32_t i = 0; i < splitedSource.size(); i++) {
		auto row = splitedSource.at(i);
		auto nums = StringHelper::Split(row, ",");
		for (std::uint32_t j = 0; j < nums.size(); j++) {
			output->data[j][i] =std::stoi( nums.at(j));
		}
	}

	return output;
}
