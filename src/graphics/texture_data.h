#pragma once

#include <cstdint>
#include <string>

#include "utils/data_buffer.h"

namespace ufps
{

enum class TextureFormat
{
    RGB,
    RGBA,
};

struct TextureData
{
    std::uint32_t width;
    std::uint32_t height;
    TextureFormat format;
    DataBuffer data;
};

inline auto to_string(TextureFormat format) -> std::string
{
    switch (format)
    {
        using enum TextureFormat;
        case RGB: return "RGB";
        case RGBA: return "RGBA";
        default: return "<\u041d\u0415\u0418\u0417\u0412\u0415\u0421\u0422\u041d\u041e>";
    }
}

}
