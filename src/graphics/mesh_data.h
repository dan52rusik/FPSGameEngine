#pragma once

#include <cstdint>
#include <format>
#include <vector>

#include "graphics/vertex_data.h"

namespace ufps
{

struct MeshData
{
    std::vector<VertexData> vertices;
    std::vector<std::uint32_t> indices;
};

inline auto to_string(const MeshData &data) -> std::string
{
    return std::format(
        "\u0434\u0430\u043d\u043d\u044b\u0435 \u043c\u0435\u0448\u0430: \u0432: {} \u0438: {}",
        data.vertices.size(),
        data.indices.size());
}

}
