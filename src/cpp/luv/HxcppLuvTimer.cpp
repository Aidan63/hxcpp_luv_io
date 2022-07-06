#include "hxcpp.h"
#include "HxcppLuvTimer.hpp"
#include "AutoGCZone.hpp"

uv_timer_t* cpp::luv::timer::repeat(uv_loop_t* loop, int interval_ms, Dynamic task)
{
    auto callback = [](uv_timer_t* timer) {
        auto gcZone = cpp::utils::AutoGCZone();
        auto task   = Dynamic{ *static_cast<hx::Object**>(timer->data) };

        task();
    };

    auto root  = new hx::Object*(task.mPtr);
    auto timer = new uv_timer_t();
    timer->data = root;

    uv_timer_init(loop, timer);
    uv_timer_start(timer, callback, interval_ms, interval_ms); 

    hx::GCAddRoot(root);

    return timer;
}

int cpp::luv::timer::stop(uv_timer_t* timer)
{
    return uv_timer_stop(timer);
}

void cpp::luv::timer::close(uv_timer_t* timer)
{
    uv_close(reinterpret_cast<uv_handle_t*>(timer), [](uv_handle_t* handle) {
        hx::GCRemoveRoot(static_cast<hx::Object**>(handle->data));

        delete handle->data;
        delete handle;
    });
}