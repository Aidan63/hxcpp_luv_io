#include <hxcpp.h>
#include <hx/Memory.h>
#include <array>

#include "HxcppLuv.hpp"
#include "AutoGCZone.hpp"

uv_loop_t* cpp::luv::allocLoop()
{
    auto loop = static_cast<uv_loop_t*>(HxAlloc(sizeof(uv_loop_t)));
    
    uv_loop_init(loop);
    
    return loop;
}

void cpp::luv::freeLoop(uv_loop_t* loop)
{
    uv_loop_close(loop);
    
    HxFree(loop);
}

bool cpp::luv::runLoop(uv_loop_t* loop, int mode)
{
    hx::AutoGCFreeZone blocking;

    return uv_run(loop, (uv_run_mode)mode) == 0;
}

String cpp::luv::errorToString(int error)
{
    return String::create(uv_strerror(error));
}