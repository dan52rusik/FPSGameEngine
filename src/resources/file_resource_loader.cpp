#include "resources/file_resource_loader.h"

#include <cstddef>
#include <errhandlingapi.h>
#include <filesystem>

#include <handleapi.h>
#include <windows.h>

#include "utils/auto_release.h"
#include "utils/data_buffer.h"
#include "utils/error.h"
#include "utils/formatter.h"
#include "utils/log.h"

namespace
{

auto init(const std::filesystem::path &path)
{
    auto handle = ufps::AutoRelease<HANDLE, nullptr>{
        ::CreateFileA(path.string().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr),
        ::CloseHandle};
    ufps::ensure(
        handle.get() != INVALID_HANDLE_VALUE,
        "\u043d\u0435 \u0443\u0434\u0430\u043b\u043e\u0441\u044c \u043e\u0442\u043a\u0440\u044b\u0442\u044c \u0444\u0430\u0439\u043b: {} \u043e\u0448\u0438\u0431\u043a\u0430: {}",
        path.string(),
        ::GetLastError());

    auto mapping = ufps::AutoRelease<HANDLE, nullptr>{
        ::CreateFileMappingA(handle, nullptr, PAGE_READONLY, 0, 0, nullptr), ::CloseHandle};
    ufps::ensure(
        mapping,
        "\u043d\u0435 \u0443\u0434\u0430\u043b\u043e\u0441\u044c \u043e\u0442\u043e\u0431\u0440\u0430\u0437\u0438\u0442\u044c \u0444\u0430\u0439\u043b \u0432 \u043f\u0430\u043c\u044f\u0442\u044c: {} \u043e\u0448\u0438\u0431\u043a\u0430: {}",
        path.string(),
        ::GetLastError());

    auto map_view = std::unique_ptr<void, decltype(&::UnmapViewOfFile)>{
        ::MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, 0), ::UnmapViewOfFile};
    ufps::ensure(
        map_view,
        "\u043d\u0435 \u0443\u0434\u0430\u043b\u043e\u0441\u044c \u043f\u043e\u043b\u0443\u0447\u0438\u0442\u044c \u043f\u0440\u0435\u0434\u0441\u0442\u0430\u0432\u043b\u0435\u043d\u0438\u0435 \u043e\u0442\u043e\u0431\u0440\u0430\u0436\u0435\u043d\u0438\u044f: {} \u043e\u0448\u0438\u0431\u043a\u0430: {}",
        path.string(),
        ::GetLastError());

    return std::make_tuple(std::move(handle), std::move(mapping), std::move(map_view));
}

template <class T>
auto load(const std::filesystem::path &path)
{
    static_assert(sizeof(typename T::value_type) == 1);

    const auto &[handle, mapping, map_view] = init(path);
    const auto size = ::GetFileSize(handle, nullptr);
    const auto *ptr = reinterpret_cast<T::value_type *>(map_view.get());

    ufps::log::info("\u0440\u0435\u0441\u0443\u0440\u0441 \u0437\u0430\u0433\u0440\u0443\u0436\u0435\u043d: {} ({})", path.string(), size);

    return T{ptr, ptr + size};
}
}

namespace ufps
{
FileResourceLoader::FileResourceLoader(const std::filesystem::path &root)
    : root_{root}
{
}

auto FileResourceLoader::load_string(std::string_view name) -> std::string
{
    return load<std::string>(root_ / name);
}

auto FileResourceLoader::load_data_buffer(std::string_view name) -> DataBuffer
{
    return load<DataBuffer>(root_ / name);
}
}
