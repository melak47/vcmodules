module;

#include <type_traits>

export module utl.meta;

export namespace utl {

    template<typename T>
    using ptr = T*;

    template<typename T>
    using nondeduced = std::common_type_t<T>;
}
