#pragma once

const size_t kSize = 8;

namespace mystdany {
    template<typename Tp>
    struct my_remove_reference {
        using type = Tp;
    };

    template<typename Tp>
    struct my_remove_reference<Tp&> {
        using type = Tp;
    };

    template<typename Tp>
    struct my_remove_reference<Tp&&> {
        using type = Tp;
    };

    template<typename Tp>
    using my_remove_reference_t = typename my_remove_reference<Tp>::type;

    class any {
    public:
        template<typename T>
        friend T any_cast(any&);

        any() noexcept {}

        template<typename T>
        any(T&& value) {
            void* addr = &buffer_;
            ::new(addr) T(value);
        }

        template<typename T>
        any& operator=(T&& value) {
            *this = any(std::forward<T>(value));

            return *this;
        }

    private:

        unsigned char buffer_[kSize];
    };

    template<typename T>
    T any_cast(any& any_) {
        using ValueType = my_remove_reference_t<T>;
        void* addr = &(any_.buffer_);
        auto p = static_cast<ValueType*>(addr);

        return static_cast<T>(*p);
    }
}

