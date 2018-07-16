#pragma once

#include <cassert>
#include <cstring>
//
#include <alias/scalar.hpp>
#include <net/net_order.hpp>

#include <iostream>

namespace net
{

class Serializer
{
    public:
    struct Array
    {
        U8   *val;
        SizeT len;
    };

    Serializer(SizeT n_bytes) :
        start(new U8[n_bytes]),
        iter(start),
        end(iter + n_bytes)
    { }

    ~Serializer() { delete[] start; }

    friend inline Serializer &operator<<(Serializer &in, U8    const &v);
    friend inline Serializer &operator<<(Serializer &in, U16   const &v);
    friend inline Serializer &operator<<(Serializer &in, U32   const &v);
    friend inline Serializer &operator<<(Serializer &in, U64   const &v);
    friend inline Serializer &operator<<(Serializer &in, Array const &v);

    SizeT get_size() const { return (SizeT)(end - start); }
    SizeT get_free() const { return (SizeT)(end - iter); }
    U8 const *get() const { return start; }

    private:
    U8 *start;
    U8 *iter;
    U8 *end;
};

inline Serializer &operator<<(Serializer &in, U8 const &v)
{
    assert(in.get_free() >= 1);
    *(in.iter) = v;
    in.iter += 1;
    return in;
}

inline Serializer &operator<<(Serializer &in, U16 const &v)
{
    U16 temp = net_order<U16>(v);
    assert(in.get_free() >= 2);
    std::memcpy(in.iter, (U8 *)&temp, 2);
    in.iter += 2;
    return in;
}

inline Serializer &operator<<(Serializer &in, U32 const &v)
{
    U32 temp = net_order<U32>(v);
    assert(in.get_free() >= 4);
    std::memcpy(in.iter, (U8 *)&temp, 4);
    in.iter += 4;
    return in;
}

inline Serializer &operator<<(Serializer &in, U64 const &v)
{
    U64 temp = net_order<U64>(v);
    assert(in.get_free() >= 8);
    std::memcpy(in.iter, (U8 *)&temp, 8);
    in.iter += 8;
    return in;
}

inline Serializer &operator<<(Serializer &in, Serializer::Array const &v)
{
    assert(in.get_free() >= v.len);
    std::memcpy(in.iter, v.val, v.len);
    in.iter += v.len;
    return in;
}

inline Serializer &operator<<(Serializer &in, I8 const &v)
{
    in << *((U8 const *)&v);
    return in;
}

inline Serializer &operator<<(Serializer &in, I16 const &v)
{
    in << *((U16 const *)&v);
    return in;
}

inline Serializer &operator<<(Serializer &in, I32 const &v)
{
    in << *((U32 const *)&v);
    return in;
}

inline Serializer &operator<<(Serializer &in, I64 const &v)
{
    in << *((U64 const *)&v);
    return in;
}

inline Serializer &operator<<(Serializer &in, bool const &v)
{
    in << *((U8 const *)&v);
    return in;
}

inline Serializer &operator<<(Serializer &in, float const &v)
{
    in << *((U32 const *)&v);
    return in;
}

inline Serializer &operator<<(Serializer &in, double const &v)
{
    in << *((U64 const *)&v);
    return in;
}

}
