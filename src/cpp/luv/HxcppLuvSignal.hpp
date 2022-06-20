#pragma once

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

#include <uv.h>

namespace cpp::luv::signal
{
    void init(uv_loop_t*, Dynamic, Dynamic);

    void start(uv_signal_t*, int, Dynamic);

    void stop(uv_signal_t*);

    void close(uv_signal_t*);
}