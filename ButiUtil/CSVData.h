#pragma once
#include<string>
#include<memory>

namespace ButiEngine {
struct CSVData :public std::enable_shared_from_this<CSVData>
{
public:
	CSVData(int arg_size_x, int arg_size_y) {
		size_x = arg_size_x;
		size_y = arg_size_y;
		data = new int* [size_x];
		for (int i = 0; i < size_x; i++) {
			data[i] = new int[size_y];
		}
	}
	~CSVData() {
		for (int i = 0; i < size_x; i++) {
			delete[] data[i];
		}
		delete[] data;
	}
	CSVData(const CSVData& obj) {
		size_x = obj.size_x;
		size_y = obj.size_y;
		data = new int* [size_x];
		for (int i = 0; i < size_x; i++) {
			data[i] = new int[size_y];
		}

		for (int i = 0; i < size_x; i++) {
			for (int j = 0; j < size_y; j++)
			{
				data[i][j] = obj.data[i][j];
			}
		}
	}
	int** data;
	int size_x;
	int size_y;
};

class CSVReader
{
public:
	BUTIUTIL_API ~CSVReader();
	BUTIUTIL_API static std::shared_ptr<CSVData> GetMatrix(const std::string& filePath);
private:
	BUTIUTIL_API CSVReader();
};
class CSVWriter
{
public:
	BUTIUTIL_API ~CSVWriter();
	BUTIUTIL_API static void Write(CSVData arg_csvData, const std::string& filePath);
private:
	BUTIUTIL_API CSVWriter();
};
}