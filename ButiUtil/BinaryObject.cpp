#include"stdafx.h"
#include "BinaryObject.h"
#include"../../zlib/zlib.h"
#pragma comment(lib,"ZDll.lib")
#include"Util.h"
#include<iostream>


std::string ButiEngine::BinaryReader_Memory::ReadString()
{
	std::int32_t size = ReadVariable<std::int32_t>();
	return ReadCharactor(size);
}

std::string ButiEngine::BinaryReader_Memory::ReadString_All()
{
	return  std::string(m_headPtr, m_headPtr+m_size);
}

std::string ButiEngine::BinaryReader_Memory::ReadCharactor(const std::uint32_t count)
{
	char* readChars = (char*)malloc(count);
	memcpy_s(readChars, count, m_current, count);

	std::string out;

	for (std::uint32_t i = 0; i < count; i++) {
		if (readChars[i] > 9) {
			out += readChars[i];
			continue;
		}
		out += readChars[i];
	}
	delete readChars;
	return out;
}

char* ButiEngine::BinaryReader_Memory::ReadCharactor()
{
	char* out = reinterpret_cast<char*>(malloc(GetReamainSize()));
	memcpy_s(out, GetReamainSize(),m_current, GetReamainSize());
	m_currentIndex += GetReamainSize();
	return out;
}

void* ButiEngine::BinaryReader_Memory::ReadData(const std::int32_t size)
{
	std::int32_t readSize = size < 0 ? GetReamainSize() : size;
	void* out = malloc(readSize);
	memcpy_s ((char*)out, readSize,m_current,readSize);
	m_current+= readSize;
	return out;
}

void ButiEngine::BinaryReader_Memory::ReadData(char* out, const std::int32_t size)
{
}

void ButiEngine::BinaryReader_Memory::ReadDefrateData(const std::uint32_t arg_compressedSize, std::uint32_t uncompressedSize, const std::uint32_t arraySize, unsigned char* outBuffer)
{
}

std::wstring ButiEngine::BinaryReader_Memory::ReadWCharactor(const std::uint32_t count)
{
	return std::wstring();
}

std::wstring ButiEngine::BinaryReader_Memory::ReadShift_jis(const std::uint32_t count)
{
	return std::wstring();
}


bool ButiEngine::BinaryReader_File::ReadStart(const std::string & filePath)
{
	fin=std::ifstream (filePath, std::ios::in | std::ios::binary);
	if (!fin) {
		std::cout<< filePath << "‚ªŒ©‚Â‚©‚è‚Ü‚¹‚ñ"<<std::endl;

		return false;
	}
	return true;
}

void ButiEngine::BinaryReader_File::ReadEnd()
{
	fin.close();
}

std::string ButiEngine::BinaryReader_File::ReadString()
{
	std::int32_t size =ReadVariable<std::int32_t>();
	return ReadCharactor(size);
}

std::string ButiEngine::BinaryReader_File::ReadString_All()
{
	return  std::string(std::istreambuf_iterator<char>(fin),std::istreambuf_iterator<char>());
}

std::string ButiEngine::BinaryReader_File::ReadCharactor(const std::uint32_t count)
{
	char* readChars=(char*)malloc(count);

	fin.read(readChars, count);

	std::string out;

	for (std::uint32_t i = 0; i <count; i++) {
		if (readChars[i] > 9) {
			out += readChars[i];
			continue;
		}
		out += readChars[i];
	}
	delete readChars;
	return out;
}

char* ButiEngine::BinaryReader_File::ReadCharactor()
{
	auto all= std::string(std::istreambuf_iterator<char>(fin),
		std::istreambuf_iterator<char>());
	char* out = (char*)malloc(all.size());
	memcpy(out, all.c_str(), all.size());

	return out;
}

void* ButiEngine::BinaryReader_File::ReadData(const std::int32_t size)
{
	std::int32_t readSize = size < 0? GetReamainSize():  size;
	void* out = malloc(readSize);
	fin.read((char*)out, readSize);
	return out;
}

void ButiEngine::BinaryReader_File::ReadData(char* out, const std::int32_t size)
{
	std::int32_t readSize = size < 0 ? GetReamainSize() : size;
	fin.read(out, readSize);
}

void ButiEngine::BinaryReader_File::ReadDefrateData(const std::uint32_t arg_compressedSize, std::uint32_t uncompressedSize, const std::uint32_t arraySize, unsigned char* outBuffer)
{
	unsigned char* inBuffer;
	inBuffer = (unsigned char*)malloc(arg_compressedSize);

	z_stream z;


	z.zalloc = Z_NULL;
	z.zfree = Z_NULL;
	z.opaque = Z_NULL;

	std::int32_t res = inflateInit(&z);
	z.next_in = NULL;
	z.avail_in = 0;
	z.next_out = outBuffer;
	z.avail_out = uncompressedSize;

	fin.read((char*)inBuffer, arg_compressedSize);

	z.next_in = inBuffer;
	z.avail_in = arg_compressedSize;

	res = inflate(&z, Z_NO_FLUSH);


	inflateEnd(&z);

	free(inBuffer);
}


std::wstring ButiEngine::BinaryReader_File::ReadWCharactor(const std::uint32_t count)
{
	wchar_t* readChars = (wchar_t*)malloc(count* sizeof(wchar_t));

	fin.read((char*)readChars, count*sizeof(wchar_t));

	std::wstring out;

	for (std::uint32_t i = 0; i < count; i++) {
		if (!readChars[i]) {
			break;
		}
		out +=readChars[i];
	}
	free( readChars);
	return out;
}

std::wstring ButiEngine::BinaryReader_File::ReadShift_jis(const std::uint32_t count)
{
	char* readChars = (char*)malloc(count * sizeof(char));

	fin.read(readChars, count * sizeof(char));

	std::string out = std::string(readChars);
	
	
	free(readChars);
	return Util::StringToWString(out);
}

std::int32_t ButiEngine::BinaryReader_File::GetReamainSize()
{
	auto current = fin.tellg();
	fin.seekg(0,std::ios::end);
	auto output = fin.tellg() - current;
	fin.seekg(current);
	return (std::int32_t)output;
}

std::int32_t ButiEngine::BinaryHelper::SwapByte(const std::int32_t arg_int)
{
	return Swap32bit(arg_int);
}

std::uint32_t ButiEngine::BinaryHelper::SwapByte(const std::uint32_t arg_UINT)
{
	return Swap32bit(arg_UINT);
}

float ButiEngine::BinaryHelper::SwapByte(const float arg_float)
{
	float output;
	std::uint32_t reverseValue;
	memcpy(&reverseValue, &arg_float, sizeof(arg_float));
	reverseValue = Swap32bit(reverseValue);
	memcpy(&output, &reverseValue, sizeof(reverseValue));
	return output;
}

double ButiEngine::BinaryHelper::SwapByte(const double & arg_double)
{
	float output;
	std::uint64_t reverseValue;
	memcpy(&reverseValue, &arg_double, sizeof(arg_double));
	reverseValue = Swap64bit(reverseValue);
	memcpy(&output, &reverseValue, sizeof(reverseValue));
	return output;
}

std::int16_t ButiEngine::BinaryHelper::Swap16bit(const std::int16_t input)
{
	std::int16_t output;
	char *conv = (char*)& input;
	char *ret = (char*)& output;


	ret[0] = conv[1];
	ret[1] = conv[0];
	return output;
}

std::int32_t ButiEngine::BinaryHelper::Swap32bit(const std::int32_t input)
{
	std::int32_t output;
	char *conv = (char*)& input;
	char *ret = (char*)& output;


	ret[0] = conv[3];
	ret[1] = conv[2];
	ret[2] = conv[1];
	ret[3] = conv[0];
	return output;
}

std::int64_t ButiEngine::BinaryHelper::Swap64bit(const std::int64_t & input)
{
	std::int64_t output;
	char *conv = (char*)& input;
	char *ret = (char*)& output;


	ret[0] = conv[7];
	ret[1] = conv[6];
	ret[2] = conv[5];
	ret[3] = conv[4];
	ret[4] = conv[3];
	ret[5] = conv[2];
	ret[6] = conv[1];
	ret[7] = conv[0];
	return output;
}

std::uint16_t ButiEngine::BinaryHelper::Swap16bit(const std::uint16_t & input)
{
	std::uint16_t output;
	char *conv = (char*)& input;
	char *ret = (char*)& output;


	ret[0] = conv[1];
	ret[1] = conv[0];
	return output;
}

std::uint32_t ButiEngine::BinaryHelper::Swap32bit(const std::uint32_t input)
{
	std::int32_t output;
	char *conv = (char*)& input;
	char *ret = (char*)& output;


	ret[0] = conv[3];
	ret[1] = conv[2];
	ret[2] = conv[1];
	ret[3] = conv[0];
	return output;
}

std::uint64_t ButiEngine::BinaryHelper::Swap64bit(const std::uint64_t & input)
{
	std::uint64_t output;
	char *conv = (char*)& input;
	char *ret = (char*)& output;


	ret[0] = conv[7];
	ret[1] = conv[6];
	ret[2] = conv[5];
	ret[3] = conv[4];
	ret[4] = conv[3];
	ret[5] = conv[2];
	ret[6] = conv[1];
	ret[7] = conv[0];
	return output;
}


bool ButiEngine::BinaryWriter_File::WriteStart(const std::string & filePath)
{
	fout = std::ofstream(filePath, std::ios::out | std::ios::binary);
	if (!fout) {
		std::cout<< filePath << "‚ªŒ©‚Â‚©‚è‚Ü‚¹‚ñ"<<std::endl;
		return false;
	}
	return true;
}

void ButiEngine::BinaryWriter_File::WriteEnd()
{
	fout.close();
}

void ButiEngine::BinaryWriter_File::WriteString(const std::string& write)
{
	WriteVariable<std::int32_t>(write.size());
	fout.write(write.c_str(), write.size());
}

void ButiEngine::BinaryWriter_File::WriteCharactor(const std::string & write)
{
	fout.write(write.c_str(), write.size());
}

void ButiEngine::BinaryWriter_File::WriteCharactor(const char* write, const std::uint32_t size)
{
	fout.write(write,size);
}

void ButiEngine::BinaryWriter_File::WriteWCharactor(const std::wstring & write)
{
	fout.write((char*)write.c_str(), write.size()*sizeof(wchar_t));
}
