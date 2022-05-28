#pragma once

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

#include <uv.h>

namespace cpp::luv
{
    void write(uv_stream_t*, Array<uint8_t>, int, int, Dynamic);
}