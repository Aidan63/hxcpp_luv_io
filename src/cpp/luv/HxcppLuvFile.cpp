#include <hxcpp.h>
#include <hx/Memory.h>
#include <array>
#include <stdio.h>
#include <memory>

#include "HxcppLuvFile.hpp"
#include "AutoGCZone.hpp"

class RootedObject
{
private:
    hx::Object** rooted;

public:
    RootedObject(void* baton) : rooted(reinterpret_cast<hx::Object**>(baton)) { }
    RootedObject(hx::Object** object) : rooted(object) { }
    RootedObject(hx::Object* object) : rooted(new hx::Object*(object))
    {
        hx::GCAddRoot(rooted);
    }

    ~RootedObject()
    {
        hx::GCRemoveRoot(rooted);

        delete rooted;
    }

    operator hx::Object*() const
    {
        return *rooted;
    }
};

std::unique_ptr<uv_fs_t, void(*)(uv_fs_t*)> make_uv_fs_t(uv_fs_t* request)
{
    auto cleanup = [](uv_fs_t* request) {
        uv_fs_req_cleanup(request);

        delete request;
    };

    return std::unique_ptr<uv_fs_t, void(*)(uv_fs_t*)>{ request, cleanup };
}

void cpp::luv::file::open(uv_loop_t* loop, String file, int flags, int mode, Dynamic callback)
{
    auto wrapper = [](uv_fs_t* request) {
        auto gcZone    = cpp::utils::AutoGCZone();
        auto object    = RootedObject(request->data);
        auto spRequest = make_uv_fs_t(request);
        auto callback  = Dynamic{ object };

        callback(request->result);
    };

    auto rootCallback = new hx::Object*(callback.mPtr);
    auto request      = new uv_fs_t();
    request->data = rootCallback;

    auto result = uv_fs_open(loop, request, file.utf8_str(), flags, mode, wrapper);
    if (result < 0)
    {
        delete rootCallback;
        delete request;

        callback(result);
    }
    else
    {
        hx::GCAddRoot(rootCallback);
    }
}

void cpp::luv::file::write(uv_loop_t* loop, uv_file file, Array<uint8_t> data, int offset, int length, Dynamic callback)
{
    auto buffer = uv_buf_init(data->getBase() + offset, length);

    if (null() == callback)
    {
        auto request = uv_fs_t();

        uv_fs_write(loop, &request, file, &buffer, 1, -1, nullptr);
    }
    else
    {
        class WriteRequest
        {
        public:
            RootedObject array;
            RootedObject callback;

            WriteRequest(hx::Object* _array, hx::Object* _callback) : array(RootedObject(_array)), callback(RootedObject(_callback)) {}
        };

        auto wrapper = [](uv_fs_t* request) {
            auto gcZone     = cpp::utils::AutoGCZone();
            auto spResult   = make_uv_fs_t(request);
            auto spData     = std::unique_ptr<WriteRequest>{ reinterpret_cast<WriteRequest*>(request->data) };
            auto callback   = Dynamic(spData->callback);

            callback(request->result);
        };

        auto request = new uv_fs_t();
        request->data = new WriteRequest(data.mPtr, callback.mPtr);

        auto result = uv_fs_write(loop, request, file, &buffer, 1, -1, wrapper);
        if (result < 0)
        {
            delete request->data;
            delete request;

            callback(result);
        }
    }
}

class ReadRequestData
{
private:
    std::unique_ptr<std::array<char, 1024>> data;
public:
    int offset;
    uv_loop_t* loop;
    const uv_file file;
    const RootedObject array;
    const RootedObject callbackSuccess;
    const RootedObject callbackError;
    const uv_buf_t buffer;

    ReadRequestData(int _offset, uv_loop_t* _loop, uv_file _file, hx::Object* _array, hx::Object* _callbackSuccess, hx::Object* _callbackError) :
        offset(_offset),
        loop(_loop),
        file(_file),
        data(std::make_unique<std::array<char, 1024>>()),
        buffer(uv_buf_init(data->data(), data->size())),
        array(RootedObject(_array)),
        callbackSuccess(RootedObject(_callbackSuccess)),
        callbackError(RootedObject(_callbackError))
    { }
};

void read_callback(uv_fs_t* request)
{
    auto gcZone = cpp::utils::AutoGCZone();

    if (request->result < 0)
    {
        // < 0 indicates an error
        auto spResult      = make_uv_fs_t(request);
        auto spRequestData = std::unique_ptr<ReadRequestData>{ reinterpret_cast<ReadRequestData*>(request->data) };
        auto callback      = Dynamic(spRequestData->callbackError);

        callback(request->result);
    }
    else if (request->result == 0)
    {
        // no more data
        auto spResult      = make_uv_fs_t(request);
        auto spRequestData = std::unique_ptr<ReadRequestData>{ reinterpret_cast<ReadRequestData*>(request->data) };
        auto callback      = Dynamic(spRequestData->callbackSuccess);
        auto array         = Dynamic(spRequestData->array);

        callback(array);
    }
    else
    {
        auto requestData = reinterpret_cast<ReadRequestData*>(request->data);
        auto array       = Array<char>(reinterpret_cast<Array_obj<char>*>((hx::Object*)requestData->array));

        // Increase the offset / position for the next request
        // Libuv doesn't have a seek function so we have to track it ourselves
        requestData->offset += request->result;

        array->memcpy(array->length, requestData->buffer.base, request->result);

        auto result = uv_fs_read(requestData->loop, request, requestData->file, &requestData->buffer, 1, requestData->offset, read_callback);
        if (result < 0)
        {
            auto spResult      = make_uv_fs_t(request);
            auto spRequestData = std::unique_ptr<ReadRequestData>{ requestData };
            auto callback      = Dynamic(spRequestData->callbackError);

            callback(result);
        }
    }
}

void cpp::luv::file::read(uv_loop_t* loop, uv_file file, int offset, Dynamic callbackSuccess, Dynamic callbackError)
{
    auto data    = new ReadRequestData(offset, loop, file, Array<char>(0, 0).mPtr, callbackSuccess.mPtr, callbackError.mPtr);
    auto request = new uv_fs_t();
    request->data = data;

    auto result = uv_fs_read(loop, request, file, &data->buffer, 1, offset, read_callback);
    if (result < 0)
    {
        delete data;
        delete request;

        callbackError(result);
    }
}

void cpp::luv::file::close(uv_loop_t* loop, uv_file file, Dynamic callback)
{
    auto wrapper = [](uv_fs_t* request) {
        auto gcZone    = cpp::utils::AutoGCZone();
        auto object    = RootedObject(request->data);
        auto spRequest = make_uv_fs_t(request);
        auto callback  = Dynamic{ object };

        callback(spRequest->result);
    };

    auto rootCallback = new hx::Object*(callback.mPtr);
    auto request      = new uv_fs_t();
    request->data = rootCallback;

    auto result = uv_fs_close(loop, request, file, wrapper);
    if (result < 0)
    {
        delete rootCallback;
        delete request;

        callback(result);
    }
    else
    {
        hx::GCAddRoot(rootCallback);
    }

}