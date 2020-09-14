export module windows.unicode;

import <string>;
import <string_view>;

export namespace windows {
    auto utf16(std::string_view utf_8) -> std::wstring;
    auto utf8(std::wstring_view utf_16) -> std::string;
}
