#include <hxcpp.h>

#include "HxcppLuvPipe.hpp"

uv_pipe_t* cpp::luv::open(uv_loop_t* loop, int file)
{
    auto pipe = new uv_pipe_t;
    uv_pipe_init(loop, pipe, false);
    uv_pipe_open(pipe, file);

    return pipe;
}