#include <cassert>
//
#include <lux/net/serializer.hpp>
#include <lux/net/deserializer.hpp>
#include <lux/net/get_size.hpp>
#include <lux/net/clear_buffer.hpp>
#include <lux/net/server/signal.hpp>

namespace net
{

using namespace server;

SizeT get_size(Signal const &v)
{
    SizeT size = get_size(v.type);
    switch(v.type)
    {
        case Signal::CONF:        size += get_size(v.conf);         break;
        case Signal::MSG:         size += get_size(v.msg);          break;
        case Signal::MAP:         size += get_size(v.map);          break;
        case Signal::MAP_UPDATES: size += get_size(v.map_updates);  break;
        default: assert(false);
    }
    return size;
}

void clear_buffer(Signal &v)
{
    clear_buffer(v.type);

    clear_buffer(v.conf);
    clear_buffer(v.msg);
    clear_buffer(v.map);
    clear_buffer(v.map_updates);
}

Serializer &operator<<(Serializer &in, Signal const &v)
{
    in << *(U8 const *)(&v.type);
    switch(v.type)
    {
        case Signal::CONF:        in << v.conf;        break;
        case Signal::MSG:         in << v.msg;         break;
        case Signal::MAP:         in << v.map;         break;
        case Signal::MAP_UPDATES: in << v.map_updates; break;
        default: assert(false);
    }
    return in;
}

Deserializer &operator>>(Deserializer &out, Signal &v)
{
    out >> *(U8 *)(&v.type);
    switch(v.type)
    {
        case Signal::CONF:        out >> v.conf;        break;
        case Signal::MSG:         out >> v.msg;         break;
        case Signal::MAP:         out >> v.map;         break;
        case Signal::MAP_UPDATES: out >> v.map_updates; break;
        default: assert(false);
    }
    return out;
}

}