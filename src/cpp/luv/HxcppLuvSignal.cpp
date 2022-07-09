#include "hxcpp.h"
#include "HxcppLuvSignal.hpp"
#include "AutoGCZone.hpp"

void cpp::luv::signal::init(uv_loop_t* loop, Dynamic success, Dynamic failure)
{
    auto signal = new uv_signal_t();
    auto result = 0;

    if ((result = uv_signal_init(loop, signal)) < 0)
    {
        delete signal;

        failure(result);
    }
    else
    {
        success(cpp::Pointer<uv_signal_t>(signal));
    }
}

void cpp::luv::signal::start(uv_signal_t* signal, int signum, Dynamic callback)
{
    auto result  = 0;
    auto wrapper = [](uv_signal_t* signal, int signnum) {
        auto gcZone   = cpp::utils::AutoGCZone();
        auto callback = Dynamic(*static_cast<hx::Object**>(signal->data));

        callback(signnum);
    };

    if (result = (uv_signal_start(signal, wrapper, signum)) < 0)
    {
        callback(result);
    }
    else
    {
        auto root = new hx::Object*(callback.mPtr);

        signal->data = root;

        hx::GCAddRoot(root);
    }
}

void cpp::luv::signal::stop(uv_signal_t* signal)
{
    uv_signal_stop(signal);

    auto root = static_cast<hx::Object**>(signal->data);

    hx::GCRemoveRoot(root);

    delete root;
}

void cpp::luv::signal::close(uv_signal_t* signal)
{
    uv_close(reinterpret_cast<uv_handle_t*>(signal), [](uv_handle_t* handle) { delete handle; });
}