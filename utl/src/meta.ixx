export module utl.meta;

import <type_traits>;

export namespace utl {

    template<typename T>
    using ptr = T*;

    template<typename T>
    using nondeduced = std::common_type_t<T>;
}
