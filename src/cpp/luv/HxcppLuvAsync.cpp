#include <hxcpp.h>
#include "HxcppLuvAsync.hpp"
#include "AutoGCZone.hpp"

uv_async_t* cpp::luv::async::init(uv_loop_t* loop, Dynamic callback)
{
    auto wrapper = [](uv_async_t* response) {
        auto gcZone = cpp::utils::AutoGCZone();
        auto object = reinterpret_cast<hx::Object**>(response->data);
        auto cb     = Dynamic(*object);

        cb();
    };

    auto async = new uv_async_t();
    auto root  = new hx::Object*(callback.mPtr);
    async->data = root;

    auto result  = 0;
    if ((result = uv_async_init(loop, async, wrapper)) < 0)
    {
        hx::Throw(String::create(uv_strerror(result)));
    }

    hx::GCAddRoot(root);

    return async;
}

int cpp::luv::async::send(uv_async_t* async)
{
    return uv_async_send(async);
}

void cpp::luv::async::close(uv_async_t* async)
{
    uv_close(reinterpret_cast<uv_handle_t*>(async), [](uv_handle_t* handle) {
        hx::GCRemoveRoot(reinterpret_cast<hx::Object**>(handle->data));

        delete handle->data;
        delete handle;
    });
}

void cpp::luv::async::unref(uv_async_t* async)
{
    uv_unref(reinterpret_cast<uv_handle_t*>(async));
}