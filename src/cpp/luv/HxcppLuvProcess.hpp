#pragma once

#include <hxcpp.h>
#include <uv.h>

namespace cpp::luv::process
{
    class SpawnData
    {
    public:
        const std::unique_ptr<std::array<char*, 2>> args;
        const std::unique_ptr<uv_process_options_t> options;
        const cpp::utils::RootedObject cbSuccess;
        const cpp::utils::RootedObject cbFailure;
        uv_pipe_t* const stdinPipe;
        uv_pipe_t* const stdoutPipe;
        uv_pipe_t* const stderrPipe;

        SpawnData(
            std::array<char*, 2>*,
            uv_process_options_t*,
            hx::Object*,
            hx::Object*,
            uv_pipe_t*,
            uv_pipe_t*,
            uv_pipe_t*);

        ~SpawnData();
    };

    void spawn(uv_loop_t*, String, hx::Anon, Dynamic, Dynamic);

    uv_stream_t* getStdioStream(uv_process_t*, int);
}