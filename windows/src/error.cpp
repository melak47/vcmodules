module;

#include <cstdint>
#include <system_error>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

module windows.error;

namespace windows {

    auto get_last_error_code() {
        return std::error_code{
            static_cast<int>(::GetLastError()),
            std::system_category()
        };
    }

    auto last_error() -> std::system_error {
        return {get_last_error_code()};
    }

    auto last_error(std::string what) -> std::system_error {
        return {get_last_error_code(), std::move(what)};
    }

    auto last_error(const char* what) -> std::system_error {
        return {get_last_error_code(), what};
    }
}
