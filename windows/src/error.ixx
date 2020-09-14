module;

#include <system_error>

export module windows.error;

export namespace windows {
    auto last_error()                 -> std::system_error;
    auto last_error(std::string what) -> std::system_error;
    auto last_error(const char* what) -> std::system_error;
}
