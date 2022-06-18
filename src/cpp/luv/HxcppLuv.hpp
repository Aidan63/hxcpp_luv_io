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

    /** util functions **/

    String errorToString(int);
}