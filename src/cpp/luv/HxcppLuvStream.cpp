#include <hxcpp.h>

#include "HxcppLuvStream.hpp"
#include "AutoGCZone.hpp"
#include "hx/Memory.h"

cpp::luv::stream::StreamData::StreamData() :
    success(nullptr),
    failure(nullptr),
    buffer(nullptr)
{
    hx::GCAddRoot(&success);
    hx::GCAddRoot(&failure);
}

cpp::luv::stream::StreamData::~StreamData()
{
    hx::GCRemoveRoot(&success);
    hx::GCRemoveRoot(&failure);
}

void cpp::luv::stream::read(uv_stream_t* _stream, Dynamic _success, Dynamic _failure)
{
    auto alloc = [](uv_handle_t* handle, size_t suggested, uv_buf_t* buffer) {
        auto gcZone = cpp::utils::AutoGCZone();
        auto data   = reinterpret_cast<StreamData*>(handle->data);

        data->buffer = std::make_unique<std::vector<char>>(suggested);

        buffer->base = data->buffer->data();
        buffer->len  = suggested;
    };

    auto read = [](uv_stream_t* stream, ssize_t read, const uv_buf_t* buffer) {
        auto gcZone = cpp::utils::AutoGCZone();
        auto data   = reinterpret_cast<StreamData*>(stream->data);

        if (read < 0)
        {
            auto callback = Dynamic(data->failure);

            callback(read);
        }
        else if (read > 0)
        {
            auto callback = Dynamic(data->success);
            auto output   = Array<char>(read, 0);

            output->memcpy(0, buffer->base, read);

            callback(output);
        }
    };

    auto result = 0;
    if ((result = uv_read_start(_stream, alloc, read)) < 0)
    {
        _failure(result);
    }
    else
    {
        auto data = reinterpret_cast<StreamData*>(_stream->data);
        data->success = _success.mPtr;
        data->failure = _failure.mPtr;
    }
}

void cpp::luv::stream::stop(uv_stream_t* _stream)
{
    uv_read_stop(_stream);
}