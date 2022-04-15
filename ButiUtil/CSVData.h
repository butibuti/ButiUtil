#pragma once
#include<string>
#include"../../ButiMemorySystem/ButiMemorySystem/ButiPtr.h"
namespace ButiEngine {
struct CSVData :public Value_ptr<CSVData>
{
public:
	CSVData(std::int32_t arg_size_x, std::int32_t arg_size_y) {
		size_x = arg_size_x;
		size_y = arg_size_y;
		data = new std::int32_t* [size_x];
		for (std::int32_t i = 0; i < size_x; i++) {
			data[i] = new std::int32_t[size_y];
		}
	}
	~CSVData() {
		for (std::int32_t i = 0; i < size_x; i++) {
			delete[] data[i];
		}
		delete[] data;
	}
	CSVData(const CSVData& obj) {
		size_x = obj.size_x;
		size_y = obj.size_y;
		data = new std::int32_t* [size_x];
		for (std::int32_t i = 0; i < size_x; i++) {
			data[i] = new std::int32_t[size_y];
		}

		for (std::int32_t i = 0; i < size_x; i++) {
			for (std::int32_t j = 0; j < size_y; j++)
			{
				data[i][j] = obj.data[i][j];
			}
		}
	}
	std::int32_t** data;
	std::int32_t size_x;
	std::int32_t size_y;
};

class CSVReader
{
public:
	~CSVReader();
	static Value_ptr<CSVData> GetMatrix(const std::string& filePath);
private:
	CSVReader();
};
class CSVWriter
{
public:
	~CSVWriter();
	static void Write(CSVData arg_csvData, const std::string& filePath);
private:
	CSVWriter();
};
}