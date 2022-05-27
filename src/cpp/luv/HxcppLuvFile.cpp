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
    return std::unique_ptr<uv_fs_t, void(*)(uv_fs_t*)>{ request, [](uv_fs_t* r) {
        uv_fs_req_cleanup(r);

        delete r;
    } };
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

void cpp::luv::file::write(uv_loop_t* loop, uv_file file, Array<uint8_t> data, Dynamic callback)
{  
    auto buffer = uv_buf_init(data->getBase(), data->length);

    auto wrapper = [](uv_fs_t* request) {
        auto gcZone     = cpp::utils::AutoGCZone();
        auto spResult   = make_uv_fs_t(request);
        auto spPair     = std::unique_ptr<std::pair<hx::Object**, hx::Object**>>{ reinterpret_cast<std::pair<hx::Object**, hx::Object**>*>(request->data) };
        auto rootObject = RootedObject(spPair->first);
        auto rootData   = RootedObject(spPair->second);
        auto callback   = Dynamic{ rootObject };

        callback(request->result);
    };

    auto rootCallback = new hx::Object*(callback.mPtr);
    auto rootData     = new hx::Object*(data.mPtr);

    auto request = new uv_fs_t();
    request->data = new std::pair<hx::Object**, hx::Object**>(rootCallback, rootData);

    auto result = uv_fs_write(loop, request, file, &buffer, 1, -1, wrapper);
    if (result < 0)
    {
        delete rootCallback;
        delete rootData;

        delete request->data;
        delete request;

        callback(result);
    }
    else
    {
        hx::GCAddRoot(rootCallback);
        hx::GCAddRoot(rootData);
    }
}

class ReadRequestData
{
private:
    std::array<char, 1024> data;
public:
    uv_loop_t* loop;
    uv_file file;
    hx::Object** array;
    hx::Object** callbackSuccess;
    hx::Object** callbackError;
    uv_buf_t buffer;

    ReadRequestData(uv_loop_t* l, uv_file f, hx::Object* a, hx::Object* cbSuccess, hx::Object* cbError) :
        loop(l),
        file(f),
        data(std::array<char, 1024>()),
        buffer(uv_buf_init(data.data(), data.size())),
        array(new hx::Object*(a)),
        callbackSuccess(new hx::Object*(cbSuccess)),
        callbackError(new hx::Object*(cbError))
    {
        hx::GCAddRoot(array);
        hx::GCAddRoot(callbackSuccess);
        hx::GCAddRoot(callbackError);
    }

    ~ReadRequestData()
    {
        hx::GCRemoveRoot(array);
        hx::GCRemoveRoot(callbackSuccess);
        hx::GCRemoveRoot(callbackError);

        delete array;
        delete callbackSuccess;
        delete callbackError;
    }
};

void read_callback(uv_fs_t* request)
{
    if (request->result < 0)
    {
        auto gcZone        = cpp::utils::AutoGCZone();
        auto spResult      = make_uv_fs_t(request);
        auto spRequestData = std::unique_ptr<ReadRequestData>{ reinterpret_cast<ReadRequestData*>(request->data) };
        auto callback      = Dynamic{ *spRequestData->callbackError };

        callback(request->result);
    }
    else if (request->result == 0)
    {
        // no more data, invoke the haxe callback and cleanup
        auto gcZone        = cpp::utils::AutoGCZone();
        auto spResult      = make_uv_fs_t(request);
        auto spRequestData = std::unique_ptr<ReadRequestData>{ reinterpret_cast<ReadRequestData*>(request->data) };
        auto callback      = Dynamic{ *spRequestData->callbackSuccess };
        auto array         = Dynamic{ *spRequestData->array };

        callback(array);
    }
    else
    {
        auto gcZone      = cpp::utils::AutoGCZone();
        auto requestData = reinterpret_cast<ReadRequestData*>(request->data);
        auto array       = Array<char>{ reinterpret_cast<Array_obj<char>*>(*requestData->array) };
        auto baseLength  = array->length;
        auto newLength   = baseLength + request->result;

        array->EnsureSize(newLength);

        for (auto i = 0; i < request->result; i++)
        {
            array[baseLength + i] = requestData->buffer.base[i];
        }

        auto result = uv_fs_read(requestData->loop, request, requestData->file, &requestData->buffer, 1, -1, read_callback);
        if (result < 0)
        {
            auto spResult      = make_uv_fs_t(request);
            auto spRequestData = std::unique_ptr<ReadRequestData>{ reinterpret_cast<ReadRequestData*>(request->data) };
            auto callback      = Dynamic{ *spRequestData->callbackError };

            callback(result);
        }
    }
}

void cpp::luv::file::read(uv_loop_t* loop, uv_file file, Dynamic callbackSuccess, Dynamic callbackError)
{
    auto data    = new ReadRequestData(loop, file, Array<char>(0, 0).mPtr, callbackSuccess.mPtr, callbackError.mPtr);
    auto request = new uv_fs_t();
    request->data = data;

    auto result = uv_fs_read(loop, request, file, &data->buffer, 1, -1, read_callback);
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