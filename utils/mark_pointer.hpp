#pragma once

#include <atomic>
#include <cstddef>

namespace utils_tm {
namespace mark {


    template<size_t i>
    inline constexpr size_t flag()
    {
        return (1ull << (63-i));
    }

    template<size_t i>
    inline constexpr size_t mask()
    {
        return ~flag<i>();
    }

    template<size_t i>
    inline constexpr size_t lower()
    {
        return flag<i>()-1;
    }


    template<size_t i, class T=void >
    inline bool atomic_mark(std::atomic<T*>& tar, T*& exp)
    {
        auto temp = (T*)(size_t(exp) | flag<i>());
        return tar.compare_exchange_strong(exp, temp);
    }

    template<size_t i, class T=void >
    inline constexpr T* mark(T* ptr)
    {
        return (T*)(size_t(ptr) | flag<i>());
    }


    template<size_t i, class T=void >
    inline bool atomic_unmark_cas(std::atomic<T*>& tar, T* exp)
    {
        return tar.compare_exchange_strong(exp, exp & mask<i>());
    }

    template<size_t i, class T=void >
    inline bool atomic_unmark(std::atomic<T*>& tar)
    {
        return tar.fetch_and(mask<i>()) & flag<i>();
    }

    template<size_t i, class T=void >
    inline constexpr T* unmark(T* ptr)
    {
        return (T*)(size_t(ptr) & mask<i>());
    }


    template<size_t i, class T=void >
    inline bool atomic_clear(std::atomic<T*>& tar)
    {
        return tar.fetch_and(lower<15>());
    }

    template<class T=void>
    inline constexpr T* clear(T* ptr)
    {
        return (T*)(size_t(ptr) & lower<15>());
    }


    template<size_t i, class T=void>
    inline constexpr bool get_mark(T* ptr)
    {
        return size_t(ptr) & flag<i>();
    }

    template<class T=void>
    inline constexpr bool is_marked(T* ptr)
    {
        return bool(size_t(ptr) & (~lower<15>()));
    }

};
};
