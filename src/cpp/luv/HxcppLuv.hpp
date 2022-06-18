#pragma once

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

#include <uv.h>

namespace cpp::luv
{
    uv_loop_t* allocLoop();
    
    void freeLoop(uv_loop_t*);

    bool runLoop(uv_loop_t*, int);

    /** event loop functions **/

    uv_timer_t* queue_repeat_task(uv_loop_t*, int, Dynamic);

    int timer_stop(uv_timer_t*);
    
    void timer_close(uv_timer_t*);

    /** util functions **/

    String errorToString(int);
}