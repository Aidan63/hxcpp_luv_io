#include <hxcpp.h>
#include <hx/Memory.h>
#include <array>

#include "HxcppLuv.hpp"
#include "AutoGCZone.hpp"

struct uv_close_callback_data {
    hx::Object* callback;
    uv_handle_t* stdout_pipe;
    uv_handle_t* stderr_pipe;
};

uv_loop_t* cpp::luv::allocLoop()
{
    auto loop = reinterpret_cast<uv_loop_t*>(HxAlloc(sizeof(uv_loop_t)));
    
    uv_loop_init(loop);
    
    return loop;
}

void cpp::luv::freeLoop(uv_loop_t* loop)
{
    uv_loop_close(loop);
    
    HxFree(loop);
}

bool cpp::luv::runLoop(uv_loop_t* loop, int mode)
{
    hx::AutoGCFreeZone blocking;

    return uv_run(loop, (uv_run_mode)mode) == 0;
}

uv_timer_t* cpp::luv::queue_repeat_task(uv_loop_t* loop, int interval_ms, Dynamic task)
{
    auto callback = [](uv_timer_t* timer) {
        auto gcZone = cpp::utils::AutoGCZone();
        auto task   = Dynamic{ *reinterpret_cast<hx::Object**>(timer->data) };

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

int cpp::luv::timer_stop(uv_timer_t* timer)
{
    return uv_timer_stop(timer);
}

void cpp::luv::timer_close(uv_timer_t* timer)
{
    uv_close(reinterpret_cast<uv_handle_t*>(timer), [](uv_handle_t* handle) {
        hx::GCRemoveRoot(reinterpret_cast<hx::Object**>(handle->data));

        delete handle->data;
        delete handle;
    });
}

String cpp::luv::errorToString(int error)
{
    return String::create(uv_strerror(error));
}