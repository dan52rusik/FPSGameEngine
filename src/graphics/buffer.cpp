#include "graphics/buffer.h"

#include <cstddef>
#include <string_view>

#include "graphics/opengl.h"
#include "utils/auto_release.h"
#include "utils/data_buffer.h"
#include "utils/error.h"

namespace ufps
{

Buffer::Buffer(std::size_t size, std::string_view name)
    : buffer_{0u, [](auto buffer) { ::glDeleteBuffers(1, &buffer); }}
    , size_{size}
{
    ::glCreateBuffers(1, &buffer_);
    ::glNamedBufferStorage(buffer_, size, nullptr, GL_DYNAMIC_STORAGE_BIT);
    ::glObjectLabel(GL_BUFFER, buffer_, name.length(), name.data());
}

auto Buffer::write(DataBufferView data, std::size_t offset) const -> void
{
    expect(size_ >= data.size_bytes() + offset, "\u0431\u0443\u0444\u0435\u0440 \u0441\u043b\u0438\u0448\u043a\u043e\u043c \u043c\u0430\u043b");
    ::glNamedBufferSubData(
        buffer_,
        static_cast<::GLintptr>(offset),
        static_cast<::GLsizeiptr>(data.size_bytes()),
        data.data());
}

auto Buffer::native_handle() const -> ::GLuint
{
    return buffer_;
}

auto Buffer::size() const -> std::size_t
{
    return size_;
}

}
