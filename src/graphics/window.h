#pragma once

#include <cstdint>
#include <optional>
#include <string>

#include <windows.h>

#include "events/event.h"
#include "utils/auto_release.h"

namespace ufps
{

enum class WindowMode
{
    FULLSCREEN,
    WINDOWED
};

class Window
{
  public:
    Window(
        WindowMode mode,
        std::uint32_t width,
        std::uint32_t height,
        std::uint32_t x,
        std::uint32_t y,
        bool mouse_locked = true);

    ~Window() = default;

    Window(const Window &) = delete;
    auto operator=(const Window &) -> Window & = delete;

    Window(Window &&) = default;
    auto operator=(Window &&) -> Window & = default;

    auto pump_event() const -> std::optional<Event>;
    auto swap() const -> void;

    auto native_handle() const -> HWND;

    auto render_width() const -> std::uint32_t;
    auto render_height() const -> std::uint32_t;

    auto window_width() const -> std::uint32_t;
    auto window_height() const -> std::uint32_t;

    auto set_title(const std::string &title) const -> void;

    auto mode() const -> WindowMode;
    auto set_mode(WindowMode mode) -> void;

  private:
    AutoRelease<HWND> window_;
    AutoRelease<HDC> dc_;
    WNDCLASSW wc_;
    std::uint32_t width_;
    std::uint32_t height_;
    WindowMode mode_;
    bool mouse_locked_;
};

inline auto to_string(WindowMode mode) -> std::string
{
    switch (mode)
    {
        using enum WindowMode;
        case FULLSCREEN: return "\u041f\u041e\u041b\u041d\u041e\u042d\u041a\u0420\u0410\u041d\u041d\u042b\u0419";
        case WINDOWED: return "\u041e\u041a\u041e\u041d\u041d\u042b\u0419";
        default: return "<\u041d\u0415\u0418\u0417\u0412\u0415\u0421\u0422\u041d\u041e>";
    }
}

}
