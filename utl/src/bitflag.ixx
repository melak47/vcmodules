export module utl.bitflag;

import <type_traits>;

export namespace utl {

    template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    auto underlying(const T& value) -> std::underlying_type_t<T> {
        return static_cast<std::underlying_type_t<T>>(value);
    }

    namespace bitflag_operators {

        template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
        auto operator|=(T& lhs, const T& rhs) -> T& {
            using value_type = std::underlying_type_t<T>;
            return lhs = static_cast<T>(underlying(lhs) | underlying(rhs));
        }
        template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
        auto operator|(T lhs, const T& rhs) -> T {
            return lhs |= rhs;
        }

        template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
        auto operator&(T lhs, const T& rhs) -> bool {
            using value_type = std::underlying_type_t<T>;
            return static_cast<bool>(underlying(lhs) & underlying(rhs));
        }
    }
}

