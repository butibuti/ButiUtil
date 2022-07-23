#ifndef BINARYOBJECT
#define BINARYOBJECT

#pragma once
#include"ButiMemorySystem/ButiMemorySystem/ButiList.h"
#include<vector>
#include<string>
#include <stack>
#include<fstream>
#include<sstream>
namespace ButiEngine {

extern class BinaryHelper {
public:
	template<typename T>
	static inline T SwapBytes(const T& input) {
		T output;
		char* conv = (char*)&input;
		char* ret = (char*)&output;
		auto size = sizeof(T);
		for (std::int32_t i = 0; i < size; i++) {
			ret[i] = conv[size - 1 - i];
		}
		return output;
	}

	static std::int32_t SwapByte(const std::int32_t arg_int);
	static std::uint32_t SwapByte(const std::uint32_t arg_UINT);
	static float SwapByte(const float arg_float);
	static double SwapByte(const double& arg_double);
private:

	static std::int16_t Swap16bit(const std::int16_t input);
	static std::int32_t Swap32bit(const std::int32_t input);
	static std::int64_t Swap64bit(const std::int64_t& input);

	static std::uint16_t Swap16bit(const std::uint16_t& input);
	static std::uint32_t Swap32bit(const std::uint32_t input);
	static std::uint64_t Swap64bit(const std::uint64_t& input);
	BinaryHelper();
};

class IBinaryReader {
public:
	virtual std::string ReadString() = 0;
	virtual std::string ReadString_All() = 0;
	virtual std::string ReadCharactor(const std::uint32_t count) = 0;
	virtual char* ReadCharactor() = 0;
	virtual void* ReadData(const  std::int32_t size) = 0;
	virtual void ReadData(char* out, const std::int32_t size) = 0;

	virtual void ReadDefrateData(const std::uint32_t arg_compressedSize, std::uint32_t uncompressedSize, const std::uint32_t arraySize, unsigned char* outBuffer) = 0;
	template<typename T>
	inline void ReadDefratedArrayData(const std::uint32_t arg_compressedSize, const std::uint32_t arraySize, std::vector< T>& out) {
		unsigned char* outBuffer;

		std::uint32_t uncompressedSize = arraySize * sizeof(T);

		outBuffer = (unsigned char*)malloc(uncompressedSize);

		ReadDefrateData(arg_compressedSize, uncompressedSize, arraySize, outBuffer);


		out.resize(arraySize);

		for (std::int32_t i = 0; i < arraySize; i++) {
			T var;
			var = *reinterpret_cast<T*>(&outBuffer[i * sizeof(T)]);
			out.at(i) = (var);
		}
		free(outBuffer);
	}
	template<typename T>
	inline void ReadDefratedArrayData(const std::uint32_t arg_compressedSize, const std::uint32_t arraySize, List< T>& out) {
		unsigned char* outBuffer;
		std::uint32_t uncompressedSize = arraySize * sizeof(T);
		outBuffer = (unsigned char*)malloc(uncompressedSize);
		ReadDefrateData(arg_compressedSize, uncompressedSize, arraySize, outBuffer);
		out.Resize(arraySize);
		for (std::int32_t i = 0; i < arraySize; i++) {
			T var;
			var = *reinterpret_cast<T*>(&outBuffer[i * sizeof(T)]);

			out.At(i) = (var);
		}
		free(outBuffer);
	}
	virtual std::wstring ReadWCharactor(const std::uint32_t count) = 0;
	virtual std::wstring ReadShift_jis(const std::uint32_t count) = 0;

	virtual bool IsEOF() = 0;

	virtual void Skip(const std::int32_t skipByte) = 0;

	virtual bool Skip(const  std::string& skipSource) = 0;

	template<typename T>
	inline T ReadVariable() {
		T out;
		ReadData(reinterpret_cast<char*>(&out), sizeof(T));
		BinaryHelper::SwapBytes(out);
		return out;
	}
	virtual std::int32_t ReadInt(const std::int32_t readRange) = 0;
	virtual std::uint32_t ReadUINT(const std::int32_t readRange) = 0;
	virtual std::int32_t GetReamainSize()=0;
};
extern class BinaryReader_Memory:public IBinaryReader {
public:
	BinaryReader_Memory(char* arg_ptr, const std::uint64_t arg_size):m_headPtr(arg_ptr),m_current(arg_ptr),m_size(arg_size){}
	std::string ReadString()override;
	std::string ReadString_All()override;
	std::string ReadCharactor(const std::uint32_t count)override;
	char* ReadCharactor()override;
	void* ReadData(const  std::int32_t size)override;
	void ReadData(char* out, const std::int32_t size)override;

	virtual void ReadDefrateData(const std::uint32_t arg_compressedSize, std::uint32_t uncompressedSize, const std::uint32_t arraySize, unsigned char* outBuffer)override;
	virtual std::wstring ReadWCharactor(const std::uint32_t count)override;
	virtual std::wstring ReadShift_jis(const std::uint32_t count)override;

	inline bool IsEOF()override { return m_currentIndex==m_size; }

	inline void Skip(const std::int32_t skipByte) override {
		m_currentIndex += skipByte;
		m_current += skipByte;
	}

	inline bool Skip(const  std::string& skipSource)override {

		auto currentPos = m_currentIndex;
		std::string find = "";
		std::int32_t i = 0;
		while (find != skipSource && !IsEOF())
		{
			char read = ReadVariable<char>();
			if (read == skipSource[i]) {
				i++;
				find += read;
			}
			else {
				i = 0;
				find.clear();
			}
		}
		if (find == skipSource) {
			std::int32_t size = (std::int32_t)skipSource.size();
			Skip(-(size));
			return true;
		}
		else {
			m_currentIndex = currentPos;
			m_current = m_headPtr + m_currentIndex;
			return false;
		}

	}

	inline std::int32_t ReadInt(const std::int32_t readRange)override {
		std::int32_t out;
		if (readRange == 1)
		{
			out = static_cast<std::int32_t>(ReadVariable<std::int8_t>());
		}
		else if (readRange == 2)
		{
			out = static_cast<std::int32_t>(ReadVariable<std::int16_t>());
		}
		else
		{
			out = static_cast<std::int32_t>(ReadVariable<std::int32_t>());
		}
		BinaryHelper::SwapBytes(out);
		return out;
	}
	inline std::uint32_t ReadUINT(const std::int32_t readRange)override {
		std::uint32_t out;
		if (readRange == 1)
		{
			out = static_cast<std::uint32_t>(ReadVariable<std::uint8_t>());
		}
		else if (readRange == 2)
		{
			out = static_cast<std::uint32_t>(ReadVariable<std::uint16_t>());
		}
		else
		{
			out = static_cast<std::uint32_t>(ReadVariable<std::uint32_t>());
		}
		BinaryHelper::SwapBytes(out);
		return out;
	}

	std::int32_t GetReamainSize()override { return m_size - m_currentIndex; }
private:
	char* m_headPtr,*m_current;
	std::uint64_t m_size, m_currentIndex=0;
};
extern class BinaryReader_File :public IBinaryReader
{
public:
	BinaryReader_File() {}
	BinaryReader_File(const std::string& arg_filePath) { ReadStart(arg_filePath); }
	~BinaryReader_File();
	bool ReadStart(const std::string& arg_filePath);
	std::string ReadString()override;
	std::string ReadString_All()override;
	std::string ReadCharactor(const std::uint32_t count)override;
	char* ReadCharactor()override;
	void* ReadData(const  std::int32_t size)override;
	void ReadData(char* out, const std::int32_t size)override;

	virtual void ReadDefrateData(const std::uint32_t arg_compressedSize, std::uint32_t uncompressedSize, const std::uint32_t arraySize, unsigned char* outBuffer)override;
	virtual std::wstring ReadWCharactor(const std::uint32_t count)override;
	virtual std::wstring ReadShift_jis(const std::uint32_t count)override;

	inline bool IsEOF() {return fin.eof();}

	inline void Skip(const std::int32_t skipByte) override{
		auto currentPos = fin.tellg();
		fin.seekg((((std::int32_t)currentPos) + skipByte), std::ios_base::beg);
	}

	inline bool Skip(const  std::string& skipSource)override {

		auto currentPos = fin.tellg();
		std::string find = "";
		std::int32_t i = 0;
		while (find != skipSource && !fin.eof())
		{
			char read = ReadVariable<char>();
			if (read == skipSource[i]) {
				i++;
				find += read;
			}
			else {
				i = 0;
				find.clear();
			}
		}
		if (find == skipSource) {
			std::int32_t size = (std::int32_t)skipSource.size();
			Skip(-(size));
			return true;
		}
		else {
			fin.clear();
			fin.seekg(((std::int32_t)currentPos), std::ios_base::beg);
			return false;
		}

	}

	inline std::int32_t ReadInt(const std::int32_t readRange)override  {
		std::int32_t out;
		if (readRange == 1)
		{
			char byte1;
			fin.read((char*)&byte1, readRange);
			out = (std::int32_t)byte1;
		}
		else if (readRange == 2)
		{
			std::int16_t byte2;
			fin.read((char*)&byte2, readRange);
			out = (std::int32_t)byte2;
		}
		else
		{
			fin.read((char*)&out, 4);
		}
		BinaryHelper::SwapBytes(out);
		return out;
	}
	inline std::uint32_t ReadUINT(const std::int32_t readRange)override  {
		std::uint32_t out;
		if (readRange == 1)
		{
			unsigned char byte1;
			fin.read((char*)&byte1, readRange);
			out = (std::uint32_t)byte1;
		}
		else if (readRange == 2)
		{
			std::uint16_t byte2;
			fin.read((char*)&byte2, readRange);
			out = (std::uint32_t)byte2;
		}
		else
		{
			fin.read((char*)&out, 4);
		}
		BinaryHelper::SwapBytes(out);
		return out;
	}
	inline std::streampos GetCurrentPos()  {
		auto output = fin.tellg();
		return output;
	}
	std::int32_t GetReamainSize()override ;
private:
	std::ifstream fin;
};
extern class BinaryWriter_File {
public:
	BinaryWriter_File() {}
	BinaryWriter_File(const std::string& arg_filePath) { WriteStart(arg_filePath); }
	~BinaryWriter_File();
	bool WriteStart(const std::string& filePath);
	void WriteString(const std::string& write);
	void WriteCharactor(const std::string& write);
	void WriteCharactor(const char* write, const std::uint32_t size);
	void WriteWCharactor(const  std::wstring& write);
	void Write(const std::stringstream& arg_stream);
	template<typename T>
	inline void WriteVariable(T  writeVar) {

		BinaryHelper::SwapBytes(writeVar);

		fout.write((char*)&writeVar, sizeof(T));


	}
	inline void WriteInt(const std::int32_t write, const std::uint32_t readRange) {
		std::int32_t out = write;

		BinaryHelper::SwapBytes(out);
		if (readRange == 1)
		{
			char byte1 = write;
			fout.write((char*)&byte1, readRange);
		}
		else if (readRange == 2)
		{
			std::int16_t byte2 = write;
			fout.write((char*)&byte2, readRange);
		}
		else
		{
			fout.write((char*)&out, 4);
		}
	}
	inline void WriteUINT(const std::uint32_t write, const std::uint32_t readRange) {
		std::uint32_t out = write;

		BinaryHelper::SwapBytes(out);
		if (readRange == 1)
		{
			std::uint8_t byte1 = write;
			fout.write((char*)&byte1, readRange);
		}
		else if (readRange == 2)
		{
			std::uint16_t byte2 = write;
			fout.write((char*)&byte2, readRange);
		}
		else
		{
			fout.write((char*)&out, 4);
		}
	}
	inline void Jump(std::streampos jumpPos) {
		fout.seekp(jumpPos, std::ios_base::beg);
	}
	inline std::streampos GetCurrentPos() {
		return fout.tellp();
	}
private:
	std::ofstream fout;
};
}

#endif // !BINARYOBJECT