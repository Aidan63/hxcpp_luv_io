#pragma once

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

#include <uv.h>

namespace cpp::luv
{
    uv_loop_t* allocLoop();
    
    void freeLoop(uv_loop_t*);

    bool runLoop(uv_loop_t*, int);

    void luvSpawn(uv_loop_t*, String, Array<String>, String, Dynamic, Dynamic, Dynamic);

    void cbProcessExit(uv_process_t*, int64_t, int);

    void cbAllocBuffer(uv_handle_t*, size_t, uv_buf_t*);

    void cbReadPipe(uv_stream_t*, ssize_t, const uv_buf_t*);

    /** event loop functions **/

    uv_handle_t* queue_repeat_task(uv_loop_t*, int, Dynamic);

    bool cancel_task(uv_handle_t*);

    /** util functions **/

    String errorToString(int);
}