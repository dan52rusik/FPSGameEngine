#include "graphics/program.h"

#include <GL/gl.h>
#include <string_view>

#include "graphics/opengl.h"
#include "graphics/shader.h"
#include "third_party/opengl/glext.h"
#include "utils/error.h"

namespace
{

auto check_state(::GLuint handle, ::GLenum state, std::string_view name, std::string_view message) -> void
{
    auto res = ::GLint{};
    ::glGetProgramiv(handle, state, &res);
    if (res != GL_TRUE)
    {
        char log[512]{};
        ::glGetProgramInfoLog(handle, sizeof(log), nullptr, log);
        throw ufps::Exception("{}: {} {}", message, name, log);
    }
}

}

namespace ufps
{

Program::Program(const Shader &vertex_shader, const Shader &fragment_shader, std::string_view name)
    : handle_{}
{
    expect(vertex_shader.type() == ShaderType::VERTEX, "\u0448\u0435\u0439\u0434\u0435\u0440 \u043d\u0435 \u044f\u0432\u043b\u044f\u0435\u0442\u0441\u044f \u0432\u0435\u0440\u0448\u0438\u043d\u043d\u044b\u043c");
    expect(fragment_shader.type() == ShaderType::FRAGMENT, "\u0448\u0435\u0439\u0434\u0435\u0440 \u043d\u0435 \u044f\u0432\u043b\u044f\u0435\u0442\u0441\u044f \u0444\u0440\u0430\u0433\u043c\u0435\u043d\u0442\u043d\u044b\u043c");

    handle_ = {::glCreateProgram(), ::glDeleteProgram};
    ensure(handle_, "\u043d\u0435 \u0443\u0434\u0430\u043b\u043e\u0441\u044c \u0441\u043e\u0437\u0434\u0430\u0442\u044c OpenGL-\u043f\u0440\u043e\u0433\u0440\u0430\u043c\u043c\u0443");

    ::glObjectLabel(GL_PROGRAM, handle_, name.length(), name.data());

    ::glAttachShader(handle_, vertex_shader.native_handle());
    ::glAttachShader(handle_, fragment_shader.native_handle());
    ::glLinkProgram(handle_);
    ::glValidateProgram(handle_);

    check_state(handle_, GL_LINK_STATUS, name, "\u043d\u0435 \u0443\u0434\u0430\u043b\u043e\u0441\u044c \u0441\u043b\u0438\u043d\u043a\u043e\u0432\u0430\u0442\u044c \u043f\u0440\u043e\u0433\u0440\u0430\u043c\u043c\u0443");
    check_state(handle_, GL_VALIDATE_STATUS, name, "\u043d\u0435 \u0443\u0434\u0430\u043b\u043e\u0441\u044c \u043f\u0440\u043e\u0432\u0435\u0440\u0438\u0442\u044c \u043f\u0440\u043e\u0433\u0440\u0430\u043c\u043c\u0443");
}

auto Program::use() const -> void
{
    ::glUseProgram(handle_);
}

auto Program::native_handle() const -> ::GLuint
{
    return handle_;
}

}
