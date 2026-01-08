#include "utils/text_utils.h"

#include <string>
#include <string_view>

#include <windows.h>

#include "utils/error.h"

namespace ufps
{

auto text_widen(std::string_view str) -> std::wstring
{
    const auto num_wide_chars =
        ::MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, str.data(), static_cast<int>(str.size()), nullptr, 0);
    ensure(num_wide_chars != 0, "\u043d\u0435 \u0443\u0434\u0430\u043b\u043e\u0441\u044c \u043f\u043e\u043b\u0443\u0447\u0438\u0442\u044c \u0440\u0430\u0437\u043c\u0435\u0440 wstring \u0434\u043b\u044f \u043f\u0440\u0435\u043e\u0431\u0440\u0430\u0437\u043e\u0432\u0430\u043d\u0438\u044f: {}", str);

    auto wide_str = std::wstring(num_wide_chars, L'\0');

    ensure(
        ::MultiByteToWideChar(
            CP_UTF8,
            MB_PRECOMPOSED,
            str.data(),
            static_cast<int>(str.size()),
            wide_str.data(),
            static_cast<int>(wide_str.size())) == static_cast<int>(wide_str.size()),
        "\u043d\u0435 \u0443\u0434\u0430\u043b\u043e\u0441\u044c \u043f\u0440\u0435\u043e\u0431\u0440\u0430\u0437\u043e\u0432\u0430\u0442\u044c \u0441\u0442\u0440\u043e\u043a\u0443 \u0432 wstring: {}",
        str);

    return wide_str;
}

auto text_narrow(std::wstring_view str) -> std::string
{
    const auto num_multi_chars = ::WideCharToMultiByte(
        CP_UTF8, WC_NO_BEST_FIT_CHARS, str.data(), static_cast<int>(str.size()), nullptr, 0, nullptr, nullptr);
    ensure(num_multi_chars != 0, "\u043d\u0435 \u0443\u0434\u0430\u043b\u043e\u0441\u044c \u043f\u043e\u043b\u0443\u0447\u0438\u0442\u044c \u0440\u0430\u0437\u043c\u0435\u0440 \u0441\u0442\u0440\u043e\u043a\u0438 \u0434\u043b\u044f \u043f\u0440\u0435\u043e\u0431\u0440\u0430\u0437\u043e\u0432\u0430\u043d\u0438\u044f");

    auto narrow_str = std::string(num_multi_chars, '\0');

    ensure(
        ::WideCharToMultiByte(
            CP_UTF8,
            WC_NO_BEST_FIT_CHARS,
            str.data(),
            static_cast<int>(str.size()),
            narrow_str.data(),
            static_cast<int>(narrow_str.size()),
            nullptr,
            nullptr) == static_cast<int>(narrow_str.size()),
        "\u043d\u0435 \u0443\u0434\u0430\u043b\u043e\u0441\u044c \u043f\u0440\u0435\u043e\u0431\u0440\u0430\u0437\u043e\u0432\u0430\u0442\u044c \u0441\u0442\u0440\u043e\u043a\u0443 \u0432 utf8");

    return narrow_str;
}

}
