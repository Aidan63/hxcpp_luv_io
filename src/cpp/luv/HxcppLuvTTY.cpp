#include "hxcpp.h"
#include "HxcppLuvTTY.hpp"
#include "HxcppLuvStream.hpp"

void cpp::luv::tty::init(uv_loop_t* _loop, int _type, Dynamic _success, Dynamic _error)
{
    auto tty    = new uv_tty_t();
    auto result = 0;

    if ((result = uv_tty_init(_loop, tty, _type, 0)) < 0)
    {
        _error(result);
    }
    else
    {
        tty->data = new cpp::luv::stream::StreamData();

        _success(::cpp::Pointer<uv_tty_t>(tty));
    }
}

void cpp::luv::tty::close(uv_tty_t* _tty)
{
    uv_close(reinterpret_cast<uv_handle_t*>(_tty), [](uv_handle_t* handle) {
        delete reinterpret_cast<cpp::luv::stream::StreamData*>(handle->data);
        delete handle;
    });
}

uv_stream_t* cpp::luv::tty::asStream(uv_tty_t* _tty)
{
    return reinterpret_cast<uv_stream_t*>(_tty);
}