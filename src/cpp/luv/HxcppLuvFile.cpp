#include <hxcpp.h>
#include <hx/Memory.h>
#include <array>
#include <stdio.h>

#include "HxcppLuvFile.hpp"

void cpp::luv::file::open(uv_loop_t* loop, String file, int mode, Dynamic callback)
{
    auto wrapper = [](uv_fs_t* request) {
        auto rooted   = reinterpret_cast<hx::Object**>(request->data);
        auto callback = Dynamic{ *rooted };

        hx::ExitGCFreeZone();
        hx::GCRemoveRoot(rooted);

        callback(request->result);

        hx::EnterGCFreeZone();

        uv_fs_req_cleanup(request);

        delete rooted;
        delete request;
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
        auto pair     = reinterpret_cast<std::pair<hx::Object**, hx::Object**>*>(request->data);
        auto callback = Dynamic{ *(pair->first) };

        hx::ExitGCFreeZone();
        hx::GCRemoveRoot(pair->first);
        hx::GCRemoveRoot(pair->second);

        callback(request->result);

        hx::EnterGCFreeZone();

        uv_fs_req_cleanup(request);

        delete request;
        delete pair->first;
        delete pair->second;
        delete pair;
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
        auto rooted   = reinterpret_cast<hx::Object**>(request->data);
        auto callback = Dynamic{ *rooted };

        hx::ExitGCFreeZone();
        hx::GCRemoveRoot(rooted);

        callback();

        hx::EnterGCFreeZone();

        uv_fs_req_cleanup(request);

        delete rooted;
        delete request;
    };

    auto rootCallback = new hx::Object*(callback.mPtr);
    auto request      = new uv_fs_t();
    request->data = rootCallback;

    uv_fs_close(loop, request, file, wrapper);

    hx::GCAddRoot(rootCallback);
}