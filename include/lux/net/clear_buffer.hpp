#pragma once

#include <type_traits>

namespace net
{

template<typename T>
inline void clear_buffer(T &v)
{
    (void)v;
    static_assert(std::is_trivial<T>::value);
}

}