#pragma once

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

#include <uv.h>

namespace cpp::luv
{
    uv_pipe_t* open(uv_loop_t*, int);
}