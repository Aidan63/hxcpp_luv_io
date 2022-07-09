#pragma once

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

#include <uv.h>

namespace cpp::luv::timer
{
    uv_timer_t* repeat(uv_loop_t*, int, Dynamic);

    uv_timer_t* run(uv_loop_t*, Dynamic);

    int stop(uv_timer_t*);
    
    void close(uv_timer_t*);
}