#pragma once

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

#include <uv.h>

namespace cpp::luv::file
{
    void open(uv_loop_t*, String, int, int, Dynamic);

    void write(uv_loop_t*, uv_file, Array<uint8_t>, Dynamic);

    void read(uv_loop_t*, uv_file, Dynamic, Dynamic);

    void close(uv_loop_t*, uv_file, Dynamic);
}