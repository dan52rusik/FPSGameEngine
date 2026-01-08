#include "graphics/utils.h"

#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "graphics/texture_data.h"
#include "utils/data_buffer.h"
#include "utils/error.h"

namespace
{

auto channels_to_format(int num_channels) -> ufps::TextureFormat
{
    switch (num_channels)
    {
        case 3: return ufps::TextureFormat::RGB;
        case 4: return ufps::TextureFormat::RGBA;
    }

    throw ufps::Exception("\u043d\u0435\u043f\u043e\u0434\u0434\u0435\u0440\u0436\u0438\u0432\u0430\u0435\u043c\u043e\u0435 \u0447\u0438\u0441\u043b\u043e \u043a\u0430\u043d\u0430\u043b\u043e\u0432: {}", num_channels);
}

}

namespace ufps
{
auto load_texture(DataBufferView image_data) -> TextureData
{
    auto width = int{};
    auto height = int{};
    auto num_channels = int{};

    auto raw_data = std::unique_ptr<::stbi_uc, void (*)(void *)>{
        ::stbi_load_from_memory(
            reinterpret_cast<const ::stbi_uc *>(image_data.data()),
            image_data.size(),
            &width,
            &height,
            &num_channels,
            0),
        ::stbi_image_free};
    ensure(raw_data, "\u043d\u0435 \u0443\u0434\u0430\u043b\u043e\u0441\u044c \u0440\u0430\u0437\u043e\u0431\u0440\u0430\u0442\u044c \u0434\u0430\u043d\u043d\u044b\u0435 \u0442\u0435\u043a\u0441\u0442\u0443\u0440\u044b");

    const auto *ptr = reinterpret_cast<const std::byte *>(raw_data.get());

    return {
        .width = static_cast<std::uint32_t>(width),
        .height = static_cast<std::uint32_t>(height),
        .format = channels_to_format(num_channels),
        .data = {ptr, ptr + width * height * num_channels}};
}
}
