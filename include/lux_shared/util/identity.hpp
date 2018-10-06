#pragma once

#include <lux_shared/common.hpp>

namespace util
{

template<typename T>
struct Identity
{
    SizeT operator()(T const &v) const {
        return static_cast<SizeT>(v);
    }
};

}
