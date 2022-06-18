#pragma once

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

#include <uv.h>

namespace cpp::luv::tty
{
    void init(uv_loop_t*, int, Dynamic, Dynamic);

    void close(uv_tty_t*);

    uv_stream_t* asStream(uv_tty_t*);
}