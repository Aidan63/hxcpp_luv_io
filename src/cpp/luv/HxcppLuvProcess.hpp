#pragma once

#ifndef HXCPP_H
#include <hxcpp.h>
#endif

#include <uv.h>
#include <memory>
#include "RootedObject.hpp"

namespace cpp::luv::process
{
    class SpawnData
    {
    public:
        const std::unique_ptr<std::vector<char*>> args;
        const std::unique_ptr<std::vector<char*>> envs;
        const std::unique_ptr<uv_process_options_t> options;
        const cpp::utils::RootedObject cbSuccess;
        const cpp::utils::RootedObject cbFailure;
        uv_pipe_t* const stdinPipe;
        uv_pipe_t* const stdoutPipe;
        uv_pipe_t* const stderrPipe;

        SpawnData(
            std::vector<char*>*,
            std::vector<char*>*,
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