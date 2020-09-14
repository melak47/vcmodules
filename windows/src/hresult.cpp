module;

#include <Unknwn.h>
#include <OleAuto.h>

module windows.hresult;

import <memory>;
import <vector>;
import <string>;
import <system_error>;
import utl.memory;
import windows.com_ptr;
import windows.unicode;

namespace {
    class hresult_error_category : public std::error_category {
    public:
        virtual auto name() const noexcept -> const char* override {
            return "HRESULT";
        }

        virtual auto message(int errval) const -> std::string override {
            // HRESULT -> error message conversion adapted from http://stackoverflow.com/a/4597974/996886

            std::wstring message;
            auto facility = static_cast<WORD>(HRESULT_FACILITY(static_cast<HRESULT>(errval)));
            windows::com_ptr<IErrorInfo> info;
            if (SUCCEEDED(::GetErrorInfo(0, info.put())) && info) {
                std::unique_ptr<wchar_t, utl::function_pointer_deleter<&::SysFreeString>> str;
                if (SUCCEEDED(info->GetDescription(utl::out_ptr(str)))) {
                    message = str.get();
                }
            }
            else if (facility == FACILITY_ITF) {
                message = L"FACILITY_ITF - This error is interface specific. No further information is available.";
            }
            else {
                // attempt to treat as a standard, system error
                return std::system_category().message(errval);
            }
            return windows::utf8(message);
        }
    };

}

namespace windows {

    auto hresult_category() noexcept -> const std::error_category& {
        static const hresult_error_category category{};
        return category;
    }

    auto check_hresult(HRESULT hr) -> void {
        if (hr < 0) {
            throw std::system_error{hr, hresult_category()};
        }
    };

    auto check_hresult(HRESULT hr, const std::string& what) -> void {
        if (hr < 0) {
            throw std::system_error{hr, hresult_category(), std::move(what)};
        }
    }

    auto check_hresult(HRESULT hr, const char* what) -> void {
        if (hr < 0) {
            throw std::system_error{hr, hresult_category(), std::move(what)};
        }
    }

}
