export module utl.memory;

import <memory>;


namespace utl {

    namespace detail {
        template<typename T, typename D>
        struct out_ptr_proxy {
            using pointer = typename std::unique_ptr<T, D>::pointer;

            std::unique_ptr<T, D>& uptr;
            pointer                temp{};

            operator pointer* () { return &temp; }
            ~out_ptr_proxy() { if (temp) uptr.reset(temp); }
        };

        template<typename FnPtrT, FnPtrT FnPtr>
        struct function_pointer_deleter;

        template<typename Ret, typename... Args, Ret(*FnPtr)(Args...)>
        struct function_pointer_deleter<Ret(*)(Args...), FnPtr> {
            auto operator()(Args... args) const -> Ret {
                return std::invoke(FnPtr, args...);
            }
        };
    }

    // Adapt a unique_ptr<T, D> to APIs expecting a
    // T** "out pointer" to put a result into.
    export
    template<typename T, typename D>
    auto out_ptr(std::unique_ptr<T, D>& uptr) {
        return detail::out_ptr_proxy<T,D>{uptr};
    }

    // Take unique ownership of pointer with custom deleter
    export
    template<typename T, typename D>
    auto take_unique(T* ptr, D&& deleter) -> std::unique_ptr<T, std::remove_cvref_t<D>> {
        return {ptr, std::forward<D>(deleter)};
    }

    // Adapts a function pointer into a deleter function object suitable for std::unique_ptr */
    export
    template<auto FnPtr>
    using function_pointer_deleter = detail::function_pointer_deleter<std::decay_t<decltype(FnPtr)>, FnPtr>;
}
