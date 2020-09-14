export module windows.hresult;

import <string>;
import <system_error>;

export using HRESULT = long;
export namespace windows {
    auto hresult_category() noexcept -> const std::error_category&;
    auto check_hresult(HRESULT hr) -> void;
    auto check_hresult(HRESULT hr, const char* what) -> void;
    auto check_hresult(HRESULT hr, const std::string& what) -> void;
}
