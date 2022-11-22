#pragma once

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

#include <uv.h>

namespace cpp::luv::pipe
{
    void open(uv_loop_t*, int, Dynamic, Dynamic);

    uv_stream_t* asStream(uv_pipe_t*);
}