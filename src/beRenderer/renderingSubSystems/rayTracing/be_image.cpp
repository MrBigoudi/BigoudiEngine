#include "be_image.hpp"
#include "be_errorHandler.hpp"

#include <iostream>
#include <fstream>

namespace be{

Image::Image(uint32_t width, uint32_t height)
    :_Width(width), _Height(height){
    _Pixels = Pixels(height);
    for(size_t i=0; i<height; i++){
        _Pixels[i] = std::vector<Vector3>(width);
    }
}

Image::Image(const Pixels& pixels){
    if(pixels.empty() || pixels[0].empty()){
        ErrorHandler::handle(
            __FILE__, __LINE__, 
            ErrorCode::BAD_VALUE_ERROR, 
            "Can't initialize an image with a matrix of width or height null!\n"
        );
    }
    _Height = pixels.size();
    _Width = pixels[0].size();
    _Pixels = Pixels(_Height);
    for(size_t i=0; i<_Height; i++){
        _Pixels[i] = std::vector<Vector3>(_Width);
        for(size_t j=0; j<_Width; j++){
            _Pixels[i][j] = pixels[i][j]; // assume pixels is a rectangle with constant width
        }
    }
}

Image::Image(ImagePtr image):Image(image->getPixels()){}

Image::Image(const Image& image):Image(image.getPixels()){}

void Image::checkIndices(uint32_t x, uint32_t y) const{
    if(x>=_Width){
        ErrorHandler::handle(
            __FILE__, __LINE__,
            ErrorCode::OUT_OF_RANGE_ERROR,
            "Can't get a value at an image image abscissa greater than its width:\n\tgiven "
            + std::to_string(x) + " expected values less than " + std::to_string(_Width) + "!\n"
        );
    }
    if(y>=_Height){
        ErrorHandler::handle(
            __FILE__, __LINE__,
            ErrorCode::OUT_OF_RANGE_ERROR,
            "Can't get a value at an image image ordinate greater than its height:\n\tgiven "
            + std::to_string(y) + " expected values less than " + std::to_string(_Height) + "!\n"
        );
    }
}


const Vector3 Image::get(uint32_t x, uint32_t y) const {
    checkIndices(x, y);
    return _Pixels[y][x];
}

void Image::set(uint32_t x, uint32_t y, const Vector3& color, Color::ColorSpace space){
    checkIndices(x, y);
    switch(space){
        case Color::RGB:
            _Pixels[y][x] = color;
            break;
        case Color::SRGB:
            // Vector3 gammaCorrected = Color::linearToGamma(color);
            Vector3 gammaCorrected = color;
            _Pixels[y][x] = Color::toSRGB(gammaCorrected);
            break;
    }
}

void Image::clear(const Vector3& color){
    for(uint32_t y=0; y<_Height; y++){
        for(uint32_t x=0; x<_Width; x++){
            set(x, y, color, Color::RGB);
        }
    }
}

void Image::savePPM(const std::string& fileName) const {
    std::ofstream out(fileName.c_str());
    if (!out){
        ErrorHandler::handle(
            __FILE__, __LINE__,
            ErrorCode::IO_ERROR,
            "Cannot open the file `" + fileName + "', to store the image!\n"
        );
    }
    out << "P3" << std::endl
        << _Width << " " << _Height << std::endl
        << "255" << std::endl;
    for (size_t y = 0; y < _Height; y++){
        for (size_t x = 0; x < _Width; x++) {
            Vector3 color = get(x,y);
            float r = color.r() * 255.f;
            float g = color.g() * 255.f;
            float b = color.b() * 255.f;
            out << std::min(255u, static_cast<uint32_t>(r))<< " "
                << std::min(255u, static_cast<uint32_t>(g))<< " "
                << std::min(255u, static_cast<uint32_t>(b))<< " ";
        }
        out << std::endl;
    }
    out.close();
}


std::vector<unsigned char> Image::pixelsToVectorOfBytes(const Pixels& pixels){
    std::vector<unsigned char> res = {};
    for(const auto& row : pixels){
        for(const auto& color : row){
            float r = color.r() * 255.f;
            float g = color.g() * 255.f;
            float b = color.b() * 255.f;
            res.push_back(static_cast<unsigned char>(std::min(255u, static_cast<uint32_t>(r))));
            res.push_back(static_cast<unsigned char>(std::min(255u, static_cast<uint32_t>(g))));
            res.push_back(static_cast<unsigned char>(std::min(255u, static_cast<uint32_t>(b))));
            res.push_back(0xFF);
        }
    }
    return res;
}


}