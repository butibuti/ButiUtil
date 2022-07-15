#include"stdafx.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include"stb/stb_image_write.h"
#include"stb/stb_image.h"
#include "ImageFileIO.h"
#include"Util.h"
#include"ObjectFactory.h"


void ButiEngine::ImageFileIO::OutputPNG(const std::string& arg_filePath,const void* arg_data, const std::int32_t width, const std::int32_t height, const std::int32_t stride, const std::int32_t size)
{
    stbi_write_png(arg_filePath.c_str(), width, height, size, arg_data, stride);
}

void ButiEngine::ImageFileIO::OutputJPG(const std::string& arg_filePath,const void* arg_data, const std::int32_t width, const std::int32_t height, const std::int32_t stride, const std::int32_t size)
{
    stbi_write_jpg(arg_filePath.c_str(), width, height, size, arg_data, stride);
}

void ButiEngine::ImageFileIO::OutputBMP(const std::string& arg_filePath,const void* arg_data, const std::int32_t width, const std::int32_t height, const std::int32_t stride, const std::int32_t size)
{
    stbi_write_bmp(arg_filePath.c_str(), width, height, size, arg_data);
}

void ButiEngine::ImageFileIO::InputImageFile(const std::string& arg_filePath, TextureResourceData& arg_ref_output )
{
    arg_ref_output.rawData = stbi_load(arg_filePath.c_str(), &arg_ref_output.width, &arg_ref_output.height, &arg_ref_output.pixSize, 0);
    arg_ref_output.rowPitch = static_cast<size_t>(arg_ref_output.width * 4);
    arg_ref_output.slicePitch =static_cast<size_t>(arg_ref_output.width * arg_ref_output.height * 4);
    arg_ref_output.loadType = 0;
}

void ButiEngine::ImageFileIO::InputImage(const void* arg_data, const std::int32_t arg_length, TextureResourceData& arg_ref_output)
{
    arg_ref_output.rawData = stbi_load_from_memory(reinterpret_cast<const std::uint8_t*> (arg_data),arg_length, &arg_ref_output.width, &arg_ref_output.height, &arg_ref_output.pixSize, 0);
    arg_ref_output.rowPitch = static_cast<size_t>(arg_ref_output.width * 4);
    arg_ref_output.slicePitch = static_cast<size_t>(arg_ref_output.width * arg_ref_output.height * 4);
    arg_ref_output.loadType = 0;
}

void ButiEngine::ImageFileIO::InputTTF(const std::string& arg_filePath, const std::int32_t size, ButiFont::FontLanguage arg_lang, FontResourceData& arg_ref_output )
{
    if (!Util::ExistFile(arg_filePath)) {
        return ;
    }
    arg_ref_output.vlp_texResData = ObjectFactory::Create<TextureResourceData>();

    ButiFont::LoadTTF(arg_filePath.c_str(), size, arg_lang, &arg_ref_output.vlp_texResData->width, &arg_ref_output.vlp_texResData->height, &arg_ref_output.vlp_texResData->rawData, &arg_ref_output.p_fontInfo);

     

    
    arg_ref_output.vlp_texResData->loadType = 1;
    arg_ref_output.vlp_texResData->pixSize = sizeof(char);
    arg_ref_output.vlp_texResData->rowPitch = static_cast<size_t>(arg_ref_output.vlp_texResData->width);
    arg_ref_output.vlp_texResData->slicePitch = static_cast<size_t>(arg_ref_output.vlp_texResData->width * arg_ref_output.vlp_texResData->height);

}

ButiEngine::ImageFileIO::TextureResourceData::~TextureResourceData()
{
    if (loadType==0) {
        stbi_image_free(rawData);
    }
    else if(loadType==1){
        ButiFont::ReleaseMemory(rawData);
    }
    else {
        delete rawData;
    }
}

ButiEngine::ImageFileIO::FontResourceData::~FontResourceData()
{
    ButiFont::ReleaseMemory(p_fontInfo);
}
