#pragma once

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

#include <uv.h>

namespace cpp::luv::tcp
{
    void bind(uv_loop_t*, String, int, Dynamic, Dynamic);

    void listen(uv_tcp_t*, Dynamic);

    void accept(uv_tcp_t*, Dynamic, Dynamic);

    uv_stream_t* asStream(uv_tcp_t*);
}