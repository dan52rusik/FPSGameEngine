#include "graphics/shader.h"

#include <format>
#include <string>
#include <string_view>

#include "graphics/opengl.h"
#include "utils/auto_release.h"
#include "utils/error.h"
#include "utils/exception.h"
#include "utils/formatter.h"

namespace
{

auto to_native(ufps::ShaderType type) -> ::GLenum
{
    switch (type)
    {
        using enum ufps::ShaderType;
        case VERTEX: return GL_VERTEX_SHADER;
        case FRAGMENT: return GL_FRAGMENT_SHADER;
    }

    throw ufps::Exception("\u043d\u0435\u0438\u0437\u0432\u0435\u0441\u0442\u043d\u044b\u0439 \u0442\u0438\u043f \u0448\u0435\u0439\u0434\u0435\u0440\u0430: {}", std::to_underlying(type));
}
}

namespace ufps
{
Shader::Shader(std::string_view source, ShaderType type, std::string_view name)
    : handle_{}
    , type_(type)
{
    handle_ = AutoRelease<::GLuint>{::glCreateShader(to_native(type)), ::glDeleteShader};
    ::glObjectLabel(GL_SHADER, handle_, name.length(), name.data());

    const ::GLchar *strings[] = {source.data()};
    const ::GLint lengths[] = {static_cast<::GLint>(source.length())};

    ::glShaderSource(handle_, 1, strings, lengths);
    ::glCompileShader(handle_);

    ::GLint result{};
    ::glGetShaderiv(handle_, GL_COMPILE_STATUS, &result);

    if (result != GL_TRUE)
    {
        char log[512];
        ::glGetShaderInfoLog(handle_, sizeof(log), nullptr, log);

        ensure(result, "\u043d\u0435 \u0443\u0434\u0430\u043b\u043e\u0441\u044c \u0441\u043a\u043e\u043c\u043f\u0438\u043b\u0438\u0440\u043e\u0432\u0430\u0442\u044c \u0448\u0435\u0439\u0434\u0435\u0440 {} {}\n{}", type_, name, log);
    }
}

auto Shader::type() const -> ShaderType
{
    return type_;
}

auto Shader::native_handle() const -> ::GLuint
{
    return handle_;
}

auto to_string(ShaderType obj) -> std::string
{
    switch (obj)
    {
        using enum ufps::ShaderType;
        case VERTEX: return "VERTEX";
        case FRAGMENT: return "FRAGMENT";
    }

    throw ufps::Exception("\u043d\u0435\u0438\u0437\u0432\u0435\u0441\u0442\u043d\u044b\u0439 \u0442\u0438\u043f \u0448\u0435\u0439\u0434\u0435\u0440\u0430: {}", std::to_underlying(obj));
}
}
