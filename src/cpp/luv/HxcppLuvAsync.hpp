#pragma once

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

#include <uv.h>

namespace cpp::luv::async
{
    uv_async_t* init(uv_loop_t*, Dynamic);

    int send(uv_async_t*);

    void close(uv_async_t*);

    void unref(uv_async_t*);
}