#include <hxcpp.h>
#include <hx/Memory.h>
#include <array>

#include "HxcppLuv.hpp"

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

void cpp::luv::luvSpawn(uv_loop_t* loop, String file, Array<String> args, String directory, Dynamic cbStdout, Dynamic cbStderr, Dynamic cbComplete)
{
    // auto result = 0;

    // // process args
    // auto uv_args = std::vector<char*>();
    // uv_args.push_back(const_cast<char*>(file.utf8_str()));

    // for (auto i = 0; i < args->size(); i++)
    // {
    //     uv_args.push_back(const_cast<char*>(args[i].utf8_str()));
    // }

    // uv_args.push_back(nullptr);

    // // pipes to stream stdout and stderr
    // auto out_pipe = uv_pipe_t{ 0 };
    // auto err_pipe = uv_pipe_t{ 0 };

    // if (result = uv_pipe_init(loop, &out_pipe, 0))
    // {
    //     hx::Throw(String::create(uv_strerror(result)));
    // }
    // if (result = uv_pipe_init(loop, &err_pipe, 0))
    // {
    //     hx::Throw(String::create(uv_strerror(result)));
    // }

    // auto child_stdio = std::array<uv_stdio_container_t, 3>();
    // child_stdio[0].flags = UV_IGNORE;

    // child_stdio[1].flags       = static_cast<uv_stdio_flags>(UV_CREATE_PIPE | UV_WRITABLE_PIPE);
    // child_stdio[1].data.stream = reinterpret_cast<uv_stream_t*>(&out_pipe);
    // child_stdio[1].data.stream->data = cbStdout.mPtr;

    // child_stdio[2].flags       = static_cast<uv_stdio_flags>(UV_CREATE_PIPE | UV_WRITABLE_PIPE);
    // child_stdio[2].data.stream = reinterpret_cast<uv_stream_t*>(&err_pipe);
    // child_stdio[2].data.stream->data = cbStderr.mPtr;

    // // process options
    // auto options = uv_process_options_t{ 0 };
    // options.stdio       = child_stdio.data();
    // options.stdio_count = child_stdio.size();
    // options.file        = uv_args[0];
    // options.args        = uv_args.data();
    // options.cwd         = directory.utf8_str();
    // options.exit_cb     = cbProcessExit;

    // auto cb_data = new uv_close_callback_data {
    //     cbComplete.mPtr,
    //     reinterpret_cast<uv_handle_t*>(&out_pipe),
    //     reinterpret_cast<uv_handle_t*>(&err_pipe)
    // };

    // auto request = uv_process_t{ 0 };
    // request.data = cb_data;

    // if (result = uv_spawn(loop, &request, &options))
    // {
    //     hx::Throw(String::create(uv_strerror(result)));
    // }
    // if (result = uv_read_start(reinterpret_cast<uv_stream_t*>(&out_pipe), cbAllocBuffer, cbReadPipe))
    // {
    //     hx::Throw(String::create(uv_strerror(result)));
    // }
    // if (result = uv_read_start(reinterpret_cast<uv_stream_t*>(&err_pipe), cbAllocBuffer, cbReadPipe))
    // {
    //     hx::Throw(String::create(uv_strerror(result)));
    // }

    // hx::GCAddRoot(&cbStdout.mPtr);
    // hx::GCAddRoot(&cbStderr.mPtr);
    // hx::GCAddRoot(&cbComplete.mPtr);
    // hx::EnterGCFreeZone();

    // uv_run(loop, UV_RUN_DEFAULT);

    // hx::TryExitGCFreeZone();
    // hx::GCRemoveRoot(&cbStdout.mPtr);
    // hx::GCRemoveRoot(&cbStderr.mPtr);
    // hx::GCRemoveRoot(&cbComplete.mPtr);
}

void cpp::luv::cbProcessExit(uv_process_t* process, int64_t exit_status, int term_signal)
{
    hx::TryExitGCFreeZone();

    auto cbData   = reinterpret_cast<uv_close_callback_data*>(process->data);
    auto callback = Dynamic(cbData->callback);

    try
    {
        callback(exit_status);
    }
    catch(...)
    {
        hx::TryExitGCFreeZone();
    }

    hx::EnterGCFreeZone();

    HxFree(process->child_stdio_buffer);
    
    uv_close(reinterpret_cast<uv_handle_t*>(process), nullptr);
    uv_close(cbData->stdout_pipe, nullptr);
    uv_close(cbData->stderr_pipe, nullptr);

    delete cbData;
}

void cpp::luv::cbAllocBuffer(uv_handle_t* handle, size_t length, uv_buf_t* buffer)
{
    buffer->len  = length;
    buffer->base = reinterpret_cast<char*>(HxAlloc(length));
}

void cpp::luv::cbReadPipe(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buffer)
{
    if (nread <= 0)
    {
        return;
    }

    hx::TryExitGCFreeZone();

    auto objPtr   = reinterpret_cast<hx::Object*>(stream->data);
    auto callback = ::Dynamic(objPtr);

    auto string = String::create(buffer->base, nread);

    try
    {
        callback(string);
    }
    catch(...)
    {
        hx::TryExitGCFreeZone();
    }

    hx::EnterGCFreeZone();
}

//

uv_handle_t* cpp::luv::queue_repeat_task(uv_loop_t* loop, int interval_ms, Dynamic task)
{
    auto callback = [](uv_timer_t* t) {
        hx::ExitGCFreeZone();

        auto task = Dynamic{ *reinterpret_cast<hx::Object**>(t->data) };

        task();

        hx::EnterGCFreeZone();
    };

    auto rootTask = new hx::Object*(task.mPtr);

    auto timer = new uv_timer_t();
    timer->data     = rootTask;
    timer->close_cb = [](uv_handle_t* h) {
        auto task = reinterpret_cast<hx::Object**>(h->data);

        hx::GCRemoveRoot(task);

        delete h;
    };

    uv_timer_init(loop, timer);
    uv_timer_start(timer, callback, interval_ms, interval_ms); 

    hx::GCAddRoot(rootTask);

    return reinterpret_cast<uv_handle_t*>(timer);
}

bool cpp::luv::cancel_task(uv_handle_t* handle)
{
    auto timer = reinterpret_cast<uv_timer_t*>(handle);

    return uv_timer_stop(timer) == 0;
}

String cpp::luv::errorToString(int error)
{
    return String::create(uv_strerror(error));
}