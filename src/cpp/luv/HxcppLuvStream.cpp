#include <hxcpp.h>

#include "HxcppLuvStream.hpp"
#include "AutoGCZone.hpp"
#include "RootedObject.hpp"
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

    // We need this explicit GC free zone as sometimes the alloc call is not defered to the event loop.
    // This seems to happen when reading tty streams for example.
    // So ensure we are in a free zone for our `uv_read_start` call so the RTTI gc zone in the callback works as expected.

    hx::EnterGCFreeZone();

    auto result = 0;
    if ((result = uv_read_start(_stream, alloc, read)) < 0)
    {
        hx::ExitGCFreeZone();

        _failure(result);
    }
    else
    {
        auto data = reinterpret_cast<StreamData*>(_stream->data);
        data->success = _success.mPtr;
        data->failure = _failure.mPtr;

        hx::ExitGCFreeZone();
    }
}

void cpp::luv::stream::stop(uv_stream_t* _stream)
{
    uv_read_stop(_stream);
}

void cpp::luv::stream::write(uv_stream_t* _stream, Array<uint8_t> _buffer, Dynamic _callback)
{
    class WriteData
    {
    public:
        const cpp::utils::RootedObject array;
        const cpp::utils::RootedObject callback;

        WriteData(Array<uint8_t> _buffer, Dynamic _callback)
            : array(_buffer.mPtr), callback(_callback.mPtr) {}
    };

    auto wrapper = [](uv_write_t* request, int status) {
        auto gcZone    = cpp::utils::AutoGCZone();
        auto spRequest = std::unique_ptr<uv_write_t>{ request };
        auto spData    = std::unique_ptr<WriteData>{ reinterpret_cast<WriteData*>(spRequest->data) };
        auto callback  = Dynamic(spData->callback);
        
        callback(status);
    };

    auto request  = new uv_write_t();
    auto uvBuffer = uv_buf_init(_buffer->getBase(), _buffer->length);
    auto result   = 0;

    if ((result = uv_write(request, _stream, &uvBuffer, 1, wrapper)) < 0)
    {
        _callback(result);

        delete request;
    }
    else
    {
        request->data = new WriteData(_buffer, _callback);
    }
}