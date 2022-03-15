#ifndef BINARYREADER
#define BINARYREADER

#pragma once
#include<vector>
#include<string>
#include <stack>
#include<fstream>
namespace ButiEngine {
	using  longLong=long long;
	using ulongLong= unsigned long long ;
	using ushort= unsigned short ;

	extern class BinaryHelper {
	public:
		template<typename T>
		static inline T SwapBytes(const T& input) {
			T output;
			char *conv = (char*)& input;
			char *ret = (char*)& output;
			auto size = sizeof(T);
			for (int i = 0; i < size; i++) {
				ret[i] = conv[size - 1 - i];
			}
			return output;
		}

		static int SwapByte(const int arg_int);
		static unsigned int SwapByte(const unsigned int arg_UINT);
		static float SwapByte(const float arg_float);
		static double SwapByte(const double& arg_double);
	private:

		static short Swap16bit(const short input);
		static int Swap32bit(const int input);
		static longLong Swap64bit(const longLong& input);

		static ushort Swap16bit(const ushort& input);
		static unsigned int Swap32bit(const unsigned int input);
		static ulongLong Swap64bit(const ulongLong& input);
		BinaryHelper();
	};

	extern class BinaryReader
	{
	public:
		bool ReadStart(const std::string& filePath);
		void ReadEnd();
		std::string ReadString();
		std::string ReadString_All();
		std::string ReadCharactor(const unsigned int count);
		char* ReadCharactor();
		void* ReadData(const  int size);
		void ReadData(char* out,const int size);

		void ReadDefrateData(const unsigned int arg_compressedSize, unsigned int uncompressedSize, const unsigned int arraySize, unsigned char* outBuffer);
		template<typename T>
		inline void ReadDefratedArrayData(const unsigned int arg_compressedSize,const unsigned int arraySize, std::vector< T>& out) {
			unsigned char* outBuffer;

			unsigned int uncompressedSize = arraySize * sizeof(T);

			outBuffer = (unsigned char*)malloc(uncompressedSize);
			
			ReadDefrateData(arg_compressedSize, uncompressedSize, arraySize, outBuffer);


			out.resize(arraySize);

			for (int i = 0; i < arraySize; i++) {
				T var;
				var=* reinterpret_cast<T*>(&outBuffer[i * sizeof(T)]);

				out.at(i)=(var);
			}
			free( outBuffer);
		}
		std::wstring ReadWCharactor(const unsigned int count);
		std::wstring ReadShift_jis(const unsigned int count);
		
		inline bool IsEOF() {
			return fin.eof();
		}

		inline void Skip(const int skipByte) {
			auto currentPos = fin.tellg();
			fin.seekg((((int)currentPos) + skipByte), std::ios_base::beg);
		}

		inline bool Skip(const  std::string& skipSource) {

			auto currentPos = fin.tellg();
			std::string find = "";
			int i = 0;
			while (find!=skipSource&&!fin.eof() )
			{
				char read= ReadVariable<char>();
				if (read == skipSource[i]) {
					i++;
					find += read;
				}
				else {
					i = 0;
				}
			}
			if (find == skipSource) {
				int size =(int) skipSource.size();
				Skip(-(size));
				return true;
			}
			else {
				fin.seekg(((int)currentPos ), std::ios_base::beg);
				return false;
			}

		}

		template<typename T>
		inline T ReadVariable() {
			T out;
			fin.read((char*)&out, sizeof(T));

			BinaryHelper::SwapBytes(out);
			return out;
		}
		inline int ReadInt(const int readRange) {
			int out;
			if (readRange == 1)
			{
				char byte1;
				fin.read((char*)&byte1, readRange);
				out = (int)byte1;
			}
			else if (readRange == 2)
			{
				short byte2;
				fin.read((char*)&byte2, readRange);
				out = (int)byte2;
			}
			else
			{
				fin.read((char*)&out, 4);
			}
			BinaryHelper:: SwapBytes(out);
			return out;
		}
		inline unsigned int ReadUINT(const int readRange) {
			unsigned int out;
			if (readRange == 1)
			{
				unsigned char byte1;
				fin.read((char*)&byte1, readRange);
				out = (unsigned int)byte1;
			}
			else if (readRange == 2)
			{
				unsigned short byte2;
				fin.read((char*)&byte2, readRange);
				out = (unsigned int)byte2;
			}
			else
			{
				fin.read((char*)&out, 4);
			}
			BinaryHelper::SwapBytes(out);
			return out;
		}
		inline std::streampos GetCurrentPos() {
			return fin.tellg();
		}
		int GetReamainSize();
		BinaryReader() {};
	private:
		std::ifstream fin;
	};
	extern class BinaryWriter {
	public:
		bool WriteStart(const std::string& filePath);
		void WriteEnd();
		void WriteString(const std::string& write);
		void WriteCharactor(const std::string& write);
		void WriteCharactor(const char* write,const unsigned int size);
		void WriteWCharactor(const  std::wstring & write);

		template<typename T>
		inline void WriteVariable( T  writeVar) {

			BinaryHelper::SwapBytes(writeVar);

			fout.write((char*)&writeVar, sizeof(T));


		}
		inline void WriteInt(const int write, const unsigned int readRange) {
			int out = write;

			BinaryHelper::SwapBytes(out);
			if (readRange == 1)
			{
				char byte1 = write;
				fout.write((char*)&byte1, readRange);
			}
			else if (readRange == 2)
			{
				short byte2 = write;
				fout.write((char*)&byte2, readRange);
			}
			else
			{
				fout.write((char*)&out, 4);
			}
		}
		inline void WriteUINT(const unsigned int write, const unsigned int readRange) {
			unsigned int out = write;

			BinaryHelper::SwapBytes(out);
			if (readRange == 1)
			{
				unsigned char byte1 = write;
				fout.write((char*)&byte1, readRange);
			}
			else if (readRange == 2)
			{
				unsigned short byte2 = write;
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
		BinaryWriter() {};
	private:
		std::ofstream fout;
	};
}

#endif // !BINARYREADER