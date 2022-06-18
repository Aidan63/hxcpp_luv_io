#pragma once

#include <hxcpp.h>
#include <uv.h>
#include <memory>
#include <vector>

namespace cpp::luv::stream
{
    class StreamData
    {
    public:
        hx::Object* success;
        hx::Object* failure;
        std::unique_ptr<std::vector<char>> buffer;

        StreamData();
        ~StreamData();
    };

    void read(uv_stream_t*, Dynamic, Dynamic);

    void stop(uv_stream_t*);
}