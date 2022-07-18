#pragma once

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

#include <uv.h>
#include <memory>
#include "RootedObject.hpp"

namespace cpp::luv::process
{
    void spawn(uv_loop_t*, String, Dynamic, Dynamic, Dynamic);

    uv_stream_t* getStdioStream(uv_process_t*, int);

    void setExitCallback(uv_process_t*, Dynamic);
}