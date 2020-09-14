import windows.unicode;
import windows.com_ptr;
import windows.error;
import windows.hresult;

import utl.bitflag;
import utl.enumerate;
import utl.memory;
import utl.meta;
import utl.overload;

import <algorithm>;
import <iostream>;
import <string>;
import <string_view>;
import <variant>;
import <vector>;

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d11.h>

int wmain(int argc, wchar_t* argv[]) {
    try {
        std::vector<std::string> args;
        std::transform(argv, argv + argc, std::back_inserter(args), &windows::utf8);

        for (auto [idx, arg] : utl::enumerate(args)) {
            std::cout << "argv[" << idx << "]: " << arg << "\n";
        }

        using foo = std::variant<int, std::string>;
        foo a = 1;
        foo b = "two";
        auto visitor = utl::overload(
            [](int i) {
                std::cout << "int: " << i << '\n';
            },
            [](std::string_view s) {
                std::cout << "string: " << s << '\n';
            }
        );
        std::visit(visitor, a);
        std::visit(visitor, b);

        if (::CreateFileA("asdfxyz", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr) == INVALID_HANDLE_VALUE) {
            std::cerr << "failed to open file asdfxyz: " << windows::last_error().what() << '\n';
        }

        windows::com_ptr<ID3D11Device> device;
        windows::com_ptr<ID3D11DeviceContext> context;

        D3D_FEATURE_LEVEL feature_levels[]{ D3D_FEATURE_LEVEL_11_0 };
        windows::check_hresult(
            ::D3D11CreateDevice(
                nullptr, D3D_DRIVER_TYPE_SOFTWARE, nullptr, 0, feature_levels,
                static_cast<UINT>(std::size(feature_levels)), D3D11_SDK_VERSION,
                device.put(), nullptr, context.put()
            ),
            "D3D11CreateDevice failed"
        );
    }
    catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << '\n';
        return 1;
    }
}
