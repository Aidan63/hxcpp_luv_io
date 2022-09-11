#include "hxcpp.h"
#include "HxcppLuvTimer.hpp"
#include "AutoGCZone.hpp"
#include "RootedObject.hpp"
#include <memory>

uv_timer_t* cpp::luv::timer::repeat(uv_loop_t* loop, int interval_ms, Dynamic task)
{
    auto callback = [](uv_timer_t* timer) {
        auto gcZone = cpp::utils::AutoGCZone();
        auto task   = Dynamic(static_cast<cpp::utils::RootedObject*>(timer->data)->getObject());

        task();
    };

    auto timer = new uv_timer_t();
    timer->data = new cpp::utils::RootedObject(task.mPtr);

    uv_timer_init(loop, timer);
    uv_timer_start(timer, callback, interval_ms, interval_ms);

    return timer;
}

uv_timer_t* cpp::luv::timer::run(uv_loop_t* loop, Dynamic task)
{
    class RunData
    {
    public:
        cpp::utils::RootedObject task;
        uv_timer_t* timer;

        RunData(hx::Object* _task, uv_timer_t* _timer)
            : task(cpp::utils::RootedObject(_task))
            , timer(_timer)
        {
            //
        }

        ~RunData()
        {
            uv_close(reinterpret_cast<uv_handle_t*>(timer), [](uv_handle_t* handle) { delete handle; });
        }
    };

    auto callback = [](uv_timer_t* timer) {
        auto gcZone = cpp::utils::AutoGCZone();
        auto root   = std::unique_ptr<RunData>{ static_cast<RunData*>(timer->data) };
        auto obj    = root->task.getObject();
        auto task   = Dynamic(obj);

        task();
    };

    auto timer = new uv_timer_t();
    timer->data = new RunData(task.mPtr, timer);

    uv_timer_init(loop, timer);
    uv_timer_start(timer, callback, 0, 0);

    return timer;
}

int cpp::luv::timer::stop(uv_timer_t* timer)
{
    return uv_timer_stop(timer);
}

void cpp::luv::timer::close(uv_timer_t* timer)
{
    uv_close(reinterpret_cast<uv_handle_t*>(timer), [](uv_handle_t* handle) {
        delete static_cast<cpp::utils::RootedObject*>(handle->data);
        delete handle;
    });
}