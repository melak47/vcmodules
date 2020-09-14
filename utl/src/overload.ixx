export module utl.overload;

import <utility>;

namespace utl {
    template<typename... F>
    struct overload_set : F... {
        using F::operator()...;
    };

    template<class... F>
    overload_set(F...) -> overload_set<F...>;

    export
    template<typename... F>
    auto overload(F&&... f) -> overload_set<F...> {
        return overload_set<F...>{std::forward<F>(f)...};
    }
}
