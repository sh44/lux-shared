#pragma once

#include <type_traits>

namespace serial
{

template<typename T>
inline SizeT get_size(T const &v)
{
    static_assert(std::is_trivial<T>::value);
    return sizeof(v);
}

}
