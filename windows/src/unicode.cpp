module;

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

module windows.unicode;

import <cstdint>;
import <string_view>;
import <stdexcept>;
import windows.error;

namespace windows {

    auto utf16(std::string_view utf_8) -> std::wstring {
        if (utf_8.empty()) {
            return {};
        }

        if (utf_8.size() > std::numeric_limits<int>::max()) {
            throw std::invalid_argument("utf16 conversion from utf8 failed: string too long");
        }

        const auto required = ::MultiByteToWideChar(
            CP_UTF8, MB_ERR_INVALID_CHARS,
            utf_8.data(), static_cast<int>(utf_8.length()),
            nullptr, 0
        );
        if (required == 0) {
            throw last_error("utf16 conversion from utf8 failed");
        }

        std::wstring buffer(static_cast<std::size_t>(required), wchar_t{});

        const auto written = ::MultiByteToWideChar(
            CP_UTF8, MB_ERR_INVALID_CHARS,
            utf_8.data(), static_cast<int>(utf_8.length()),
            buffer.data(), static_cast<int>(buffer.length())
        );

        if (written != required) {
            throw std::runtime_error("utf16 conversion from utf8 failed");
        }

        return buffer;
    }

    auto utf8(std::wstring_view utf_16) -> std::string {
        if (utf_16.empty()) {
            return {};
        }

        if (utf_16.size() > std::numeric_limits<int>::max()) {
            throw std::invalid_argument("utf8 conversion from utf16 failed: string too long");
        }

        const auto required = ::WideCharToMultiByte(
            CP_UTF8, WC_ERR_INVALID_CHARS,
            utf_16.data(), static_cast<int>(utf_16.length()),
            nullptr, 0,
            nullptr, nullptr
        );
        if (required == 0) {
            throw last_error("utf8 conversion from utf16 failed");
        }

        std::string buffer(static_cast<std::size_t>(required), char{});

        const auto written = ::WideCharToMultiByte(
            CP_UTF8, WC_ERR_INVALID_CHARS,
            utf_16.data(), static_cast<int>(utf_16.length()),
            buffer.data(), static_cast<int>(buffer.length()),
            nullptr, nullptr
        );

        if (written != required) {
            throw std::runtime_error("utf8 conversion from utf16 failed");
        }

        return buffer;
    }
}
