#include "system_info.h"

#include <format>
#include <ranges>
#include <string>

#include <comdef.h>
#include <wbemidl.h>
#include <windows.h>

#include "utils/exception.h"
#include "utils/formatter.h"
#include "utils/log.h"
#include "utils/text_utils.h"
#include "utils/wmi.h"

namespace
{

auto os_version(ufps::Wmi &wmi) -> std::string
{
    try
    {
        const auto caption = wmi.query("SELECT * FROM Win32_OperatingSystem", "Caption") | std::views::join_with(' ') |
                             std::ranges::to<std::string>();
        const auto version = wmi.query("SELECT * FROM Win32_OperatingSystem", "Version") | std::views::join_with(' ') |
                             std::ranges::to<std::string>();

        return std::format("{} ({})", caption, version);
    }
    catch (ufps::Exception &e)
    {
        ufps::log::error("\u043d\u0435 \u0443\u0434\u0430\u043b\u043e\u0441\u044c \u043f\u043e\u043b\u0443\u0447\u0438\u0442\u044c \u0432\u0435\u0440\u0441\u0438\u044e \u041e\u0421: {}", e);
    }
    catch (...)
    {
        ufps::log::error("\u043d\u0435 \u0443\u0434\u0430\u043b\u043e\u0441\u044c \u043f\u043e\u043b\u0443\u0447\u0438\u0442\u044c \u0432\u0435\u0440\u0441\u0438\u044e \u041e\u0421");
    }

    return {};
}

auto gpu_id(const ufps::Wmi &wmi) -> std::string
{
    try
    {
        const auto props = wmi.query("SELECT * FROM Win32_VideoController", "Caption");
        return props | std::views::join_with(' ') | std::ranges::to<std::string>();
    }
    catch (ufps::Exception &e)
    {
        ufps::log::error("\u043d\u0435 \u0443\u0434\u0430\u043b\u043e\u0441\u044c \u043f\u043e\u043b\u0443\u0447\u0438\u0442\u044c \u0438\u0434\u0435\u043d\u0442\u0438\u0444\u0438\u043a\u0430\u0442\u043e\u0440 GPU: {}", e);
    }
    catch (...)
    {
        ufps::log::error("\u043d\u0435 \u0443\u0434\u0430\u043b\u043e\u0441\u044c \u043f\u043e\u043b\u0443\u0447\u0438\u0442\u044c \u0438\u0434\u0435\u043d\u0442\u0438\u0444\u0438\u043a\u0430\u0442\u043e\u0440 GPU");
    }

    return {};
}

auto gpu_driver(const ufps::Wmi &wmi) -> std::string
{
    try
    {
        const auto props = wmi.query("SELECT * FROM Win32_VideoController", "DriverVersion");
        return props | std::views::join_with(' ') | std::ranges::to<std::string>();
    }
    catch (ufps::Exception &e)
    {
        ufps::log::error("\u043d\u0435 \u0443\u0434\u0430\u043b\u043e\u0441\u044c \u043f\u043e\u043b\u0443\u0447\u0438\u0442\u044c \u0432\u0435\u0440\u0441\u0438\u044e \u0434\u0440\u0430\u0439\u0432\u0435\u0440\u0430 GPU: {}", e);
    }
    catch (...)
    {
        ufps::log::error("\u043d\u0435 \u0443\u0434\u0430\u043b\u043e\u0441\u044c \u043f\u043e\u043b\u0443\u0447\u0438\u0442\u044c \u0432\u0435\u0440\u0441\u0438\u044e \u0434\u0440\u0430\u0439\u0432\u0435\u0440\u0430 GPU");
    }

    return {};
}

auto system_memory() -> std::string
{
    auto mem = ::ULONGLONG{};
    ::GetPhysicallyInstalledSystemMemory(&mem);

    return mem == 0 ? "" : std::format("{}", mem);
}

}

namespace ufps
{

auto system_info() -> SystemInfo
{
    auto wmi = Wmi{};
    return {
        .os_version = os_version(wmi),
        .gpu_id = gpu_id(wmi),
        .gpu_driver = gpu_driver(wmi),
        .system_memory = system_memory()};
}

auto to_string(const SystemInfo &info) -> std::string
{
    const auto get_or_empty = [](const auto &s) { return s.empty() ? "<\u043d\u0435\u0438\u0437\u0432\u0435\u0441\u0442\u043d\u043e>" : s; };

    return std::format(
        "\u0432\u0435\u0440\u0441\u0438\u044f \u041e\u0421: {}\n\u0432\u0438\u0434\u0435\u043e\u043a\u0430\u0440\u0442\u0430: {}\n\u0434\u0440\u0430\u0439\u0432\u0435\u0440 \u0432\u0438\u0434\u0435\u043e\u043a\u0430\u0440\u0442\u044b: {}\n\u0441\u0438\u0441\u0442\u0435\u043c\u043d\u0430\u044f \u043f\u0430\u043c\u044f\u0442\u044c: {}",
        get_or_empty(info.os_version),
        get_or_empty(info.gpu_id),
        get_or_empty(info.gpu_driver),
        get_or_empty(info.system_memory));
}

};
