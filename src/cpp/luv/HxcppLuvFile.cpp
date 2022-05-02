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

void cpp::luv::file::open(uv_loop_t* loop, String file, int mode, Dynamic callback)
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

    uv_fs_open(loop, request, file.utf8_str(), mode, 0, wrapper);
    
    hx::GCAddRoot(rootCallback);
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

    uv_fs_write(loop, request, file, &buffer, 1, -1, wrapper);

    hx::GCAddRoot(rootCallback);
    hx::GCAddRoot(rootData);
}

void cpp::luv::file::read(uv_loop_t* loop, uv_file file, Dynamic callback)
{
    //
}

void cpp::luv::file::close(uv_loop_t* loop, uv_file file, Dynamic callback)
{
    auto wrapper = [](uv_fs_t* request) {
        auto gcZone    = cpp::utils::AutoGCZone();
        auto object    = RootedObject(request->data);
        auto spRequest = make_uv_fs_t(request);
        auto callback  = Dynamic{ object };

        callback();
    };

    auto rootCallback = new hx::Object*(callback.mPtr);
    auto request      = new uv_fs_t();
    request->data = rootCallback;

    uv_fs_close(loop, request, file, wrapper);

    hx::GCAddRoot(rootCallback);
}