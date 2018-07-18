#pragma once

#include <lux/alias/scalar.hpp>
#include <lux/alias/c_string.hpp>
#include <lux/alias/vector.hpp>
#include <lux/net/serializer.hpp>
#include <lux/net/deserializer.hpp>
#include <lux/net/server/chunk_data.hpp>
#include <lux/common/entity.hpp>

namespace net
{

#pragma pack(push, 1)
struct ServerInitData
{
    double              tick_rate;
    CString             server_name; //TODO max length?
    linear::Vec3<SizeT> chunk_size;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct ServerData
{
    Vector<ChunkData> chunks;
    Vector<EntityPos> entities;
    EntityPos        player_pos;
};
#pragma pack(pop)

inline Serializer &operator<<(Serializer &in, ServerData const &v)
{
    in << v.chunks;
    in << v.entities;
    in << v.player_pos;
    return in;
}

inline Deserializer &operator>>(Deserializer &out, ServerData &v)
{
    out >> v.chunks;
    out >> v.entities;
    out >> v.player_pos;
    return out;
}

}
