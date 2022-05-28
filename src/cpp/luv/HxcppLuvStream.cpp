#include <hxcpp.h>

#include "HxcppLuvStream.hpp"
#include "RootedObject.hpp"
#include <memory>

void cpp::luv::write(uv_stream_t* stream, Array<uint8_t> base, int offset, int length, Dynamic callback)
{
    class WriteData
    {
    public:
        const uv_buf_t* buffer;
        const cpp::utils::RootedObject callback;
        const cpp::utils::RootedObject array;

        WriteData(uv_buf_t* _buffer, cpp::utils::RootedObject _callback, cpp::utils::RootedObject _array) :
            buffer(_buffer),
            callback(_callback),
            array(_array)
        {
            //
        }
    };

    auto buffer = new uv_buf_t;
    buffer->base = base->getBase() + offset;
    buffer->len  = length;

    auto wrapper = [](uv_write_t* req, int status) {
        auto data     = std::unique_ptr<WriteData>{ reinterpret_cast<WriteData*>(req->data) };
        auto callback = Dynamic(data->callback);

        delete data->buffer;
        delete req;

        callback(status);
    };

    auto req = new uv_write_t;
    req->data = new WriteData(buffer, cpp::utils::RootedObject(callback.mPtr), cpp::utils::RootedObject(base.mPtr));;

    uv_write(req, stream, buffer, 1, wrapper);
}