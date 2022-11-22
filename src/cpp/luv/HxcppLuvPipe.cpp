#include "hxcpp.h"
#include "HxcppLuvPipe.hpp"
#include "HxcppLuvStream.hpp"

void cpp::luv::pipe::open(uv_loop_t* _loop, int _file, Dynamic _success, Dynamic _error)
{
    auto result = 0;
    auto pipe   = std::make_unique<uv_pipe_t>();

    if ((result = uv_pipe_init(_loop, pipe.get(), false)) < 0)
    {
        _error(result);
    }
    else
    {
        if ((result = uv_pipe_open(pipe.get(), _file)) < 0)
        {
            _error(result);
        }
        else
        {
            pipe->data = new cpp::luv::stream::StreamData();

            _success(cpp::Pointer<uv_pipe_t>(pipe.release()));
        }
    }
}

uv_stream_t* cpp::luv::pipe::asStream(uv_pipe_t* _pipe)
{
    return reinterpret_cast<uv_stream_t*>(_pipe);
}