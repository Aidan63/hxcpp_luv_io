#pragma once

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

#include <uv.h>

namespace cpp::luv::directory
{
    void open(uv_loop_t*, String, Dynamic, Dynamic);

    void read(uv_loop_t*, uv_dir_t*, Dynamic, Dynamic);
}