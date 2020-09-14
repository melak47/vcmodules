module;

#include <stdexcept>
#include <type_traits>
#include <utility>

#include <Unknwn.h>

export module windows.com_ptr;

import windows.hresult;

namespace windows {

    export
    template<typename T>
    class com_ptr {
        static_assert(std::is_base_of_v<IUnknown, T>, "T does not derive from COM IUnknown interface");
        T* ptr = nullptr;
    public:
        constexpr static const IID& uuid = __uuidof(T);

        com_ptr() = default;
        explicit com_ptr(std::nullptr_t) {}
        explicit com_ptr(T* new_ptr) : ptr{new_ptr} {}

        com_ptr(com_ptr&& other) : ptr{other.release()} {}

        com_ptr(const com_ptr& other) : ptr{other.ptr} {
            add_ref();
        }

        ~com_ptr() {
            decr_ref();
        }

        com_ptr& operator=(com_ptr&& other) {
            reset(other.release());
            return *this;
        }

        com_ptr& operator=(const com_ptr& other) {
            other.add_ref();
            reset(other.get());
            return *this;
        }

        auto get() const -> T* {
            return ptr;
        }

        explicit operator bool () const {
            return ptr != nullptr;
        }

        auto put() -> T** {
            return &ptr;
        }

        auto put_void() -> void** {
            return reinterpret_cast<void**>(&ptr);
        }

        auto put_unknown() -> IUnknown** {
            return reinterpret_cast<IUnknown**>(&ptr);
        }

        auto reset(T* new_ptr = nullptr) {
            decr_ref();
            ptr = new_ptr;
        }

        auto release() -> T* {
            return std::exchange(ptr, nullptr);
        }

        T* operator-> () const {
            if (!ptr) {
                throw std::runtime_error("dereferencing empty com_ptr");
            }
            return ptr;
        }

        T& operator* () const {
            if (!ptr) {
                throw std::runtime_error("dereferencing empty com_ptr");
            }
            return *ptr;
        }

        template<typename U>
        auto query_interface() const -> com_ptr<U> {
            com_ptr<U> result;
            if (!ptr) {
                throw std::runtime_error("query_interface empty com_ptr");
            }
            check_hresult(
                ptr->template QueryInterface<U>(result.put()),
                "QueryInterface failed"
            );
            return result;
        }

        template<typename U>
        auto try_as() const -> com_ptr<U> {
            com_ptr<U> result;
            if (!ptr)
                return result;
            ptr->template QueryInterface<U>(result.put());
            return result;
        }

        friend bool operator==(const com_ptr& lhs, const com_ptr& rhs) {
            return lhs.ptr == rhs.ptr;
        }

        friend bool operator==(const com_ptr& lhs, std::nullptr_t) {
            return lhs.ptr == nullptr;
        }

    private:
        void add_ref() {
            if (ptr) {
                ptr->AddRef();
            }
        }

        void decr_ref() {
            if (ptr) {
                ptr->Release();
            }
        }
    };

}
