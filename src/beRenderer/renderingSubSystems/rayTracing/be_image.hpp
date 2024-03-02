#pragma once

#include "be_vector3.hpp"
#include <vector>
#include <memory>

namespace be{

class Image;
using ImagePtr = std::shared_ptr<Image>;

using Pixels = std::vector<std::vector<Vector3>>;

class Image{

    private:
        uint32_t _Width = 0;
        uint32_t _Height = 0;

        Pixels _Pixels = {};

    public:
        Image(uint32_t width, uint32_t height);
        Image(const Pixels& pixels);
        Image(ImagePtr image);
        Image(const Image& image);

    public:
        uint32_t getWidth() const {return _Width;}
        uint32_t getHeight() const {return _Height;}
        const Pixels getPixels() const {return _Pixels;}
        const Vector3 get(uint32_t x, uint32_t y) const;
        void set(uint32_t x, uint32_t y, const Vector3& color);

    public:
        void clear(const Vector3& color = {});
        void savePPM(const std::string& fileName);

    private:
        void checkIndices(uint32_t x, uint32_t y) const;
};

}