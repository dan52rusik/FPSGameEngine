#include "graphics/texture.h"

#include "graphics/opengl.h"
#include "graphics/sampler.h"
#include "graphics/texture_data.h"
#include "utils/exception.h"
#include "utils/formatter.h"

namespace
{

auto to_opengl(ufps::TextureFormat format, bool include_size) -> ::GLenum
{
    switch (format)
    {
        using enum ufps::TextureFormat;
        case RGB: return include_size ? GL_RGB8 : GL_RGB;
        case RGBA: return include_size ? GL_RGBA8 : GL_RGBA;
    }

    throw ufps::Exception("\u043d\u0435\u0438\u0437\u0432\u0435\u0441\u0442\u043d\u044b\u0439 \u0444\u043e\u0440\u043c\u0430\u0442 \u0442\u0435\u043a\u0441\u0442\u0443\u0440\u044b: {}", format);
}

}

namespace ufps
{

Texture::Texture(const TextureData &texture, const std::string &name, const Sampler &sampler)
    : handle_{0u, [](auto tex) { ::glDeleteTextures(1, &tex); }}
    , bindless_handle_{}
    , name_{name}
{
    ::glCreateTextures(GL_TEXTURE_2D, 1, &handle_);
    ::glTextureStorage2D(handle_, 1, to_opengl(texture.format, true), texture.width, texture.height);
    ::glTextureSubImage2D(
        handle_,
        0,
        0,
        0,
        texture.width,
        texture.height,
        to_opengl(texture.format, false),
        GL_UNSIGNED_BYTE,
        texture.data.data());
    ::glGenerateTextureMipmap(handle_);

    ::glObjectLabel(GL_TEXTURE, handle_, name.length(), name.data());

    bindless_handle_ = ::glGetTextureSamplerHandleARB(handle_, sampler.native_handle());
    ::glMakeTextureHandleResidentARB(bindless_handle_);
}

Texture::~Texture()
{
    ::glMakeTextureHandleNonResidentARB(bindless_handle_);
}

auto Texture::native_handle() const -> ::GLuint64
{
    return bindless_handle_;
}

auto Texture::name() const -> std::string
{
    return name_;
}

}
