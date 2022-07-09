#include <hxcpp.h>
#include "HxcppLuvProcess.hpp"
#include "HxcppLuvStream.hpp"
#include "hx/StdLibs.h"
#include "RootedObject.hpp"
#include <string>
#include <vector>
#include <array>

std::vector<char*>* createLuvEnvironmentVars(Dynamic _map)
{
    auto keys = __string_hash_keys(_map);
    auto luv  = new std::vector<char*>();
    
    for (auto i = 0; i < keys->length; i++)
    {
        auto key  = keys[i];
        auto item = __string_hash_get_string(_map, key);

        if (null() != item)
        {
            luv->push_back(const_cast<char*>((std::string(key.utf8_str()) + std::string("=") + std::string(item.utf8_str())).data()));
        }
        else
        {
            luv->push_back(const_cast<char*>(key.utf8_str()));
        }
    }

    luv->push_back(nullptr);

    return luv;
}

cpp::luv::process::SpawnData::SpawnData(
    std::vector<char*>* _args,
    std::vector<char*>* _envs,
    uv_process_options_t* _options,
    hx::Object* _cbSuccess,
    hx::Object* _cbFailure,
    uv_pipe_t* _stdinPipe,
    uv_pipe_t* _stdoutPipe,
    uv_pipe_t* _stderrPipe) :
    args(_args),
    options(_options),
    envs(_envs),
    cbSuccess(_cbSuccess),
    cbFailure(_cbFailure),
    stdinPipe(_stdinPipe),
    stdoutPipe(_stdoutPipe),
    stderrPipe(_stderrPipe)
{
    //
}

cpp::luv::process::SpawnData::~SpawnData()
{
    auto callback = [](uv_handle_t* handle) {
        delete static_cast<cpp::luv::stream::StreamData*>(handle->data);
        delete handle;
    };

    uv_close(reinterpret_cast<uv_handle_t*>(stdinPipe), callback);
    uv_close(reinterpret_cast<uv_handle_t*>(stdoutPipe), callback);
    uv_close(reinterpret_cast<uv_handle_t*>(stderrPipe), callback);
}

void cpp::luv::process::spawn(uv_loop_t* _loop, String _file, hx::Anon _options, Dynamic _success, Dynamic _failure)
{
    auto stdinPipe  = new uv_pipe_t();
    auto stdoutPipe = new uv_pipe_t();
    auto stderrPipe = new uv_pipe_t();

    uv_pipe_init(_loop, stdinPipe, false);
    uv_pipe_init(_loop, stderrPipe, false);
    uv_pipe_init(_loop, stdoutPipe, false);

    stdinPipe->data = new cpp::luv::stream::StreamData();
    stdoutPipe->data = new cpp::luv::stream::StreamData();
    stderrPipe->data = new cpp::luv::stream::StreamData();

    auto stdio = new std::array<uv_stdio_container_t, 3>();
    stdio->at(0).flags = static_cast<uv_stdio_flags>(UV_CREATE_PIPE | UV_READABLE_PIPE);
    stdio->at(1).flags = static_cast<uv_stdio_flags>(UV_CREATE_PIPE | UV_WRITABLE_PIPE);
    stdio->at(2).flags = static_cast<uv_stdio_flags>(UV_CREATE_PIPE | UV_WRITABLE_PIPE);

    stdio->at(0).data.stream = reinterpret_cast<uv_stream_t*>(stdinPipe);
    stdio->at(1).data.stream = reinterpret_cast<uv_stream_t*>(stdoutPipe);
    stdio->at(2).data.stream = reinterpret_cast<uv_stream_t*>(stderrPipe);

    auto hxCwd  = _options->__Field("cwd", HX_PROP_ALWAYS).asString();
    auto hxArgs = Array<String>(RawAutoCast(_options->__Field("args", HX_PROP_ALWAYS).asObject()));
    auto hxEnv  = _options->__Field("env", HX_PROP_ALWAYS).asObject()->__Field("h", HX_PROP_ALWAYS).asObject();

    // arg 0 must be the program, and the final one must be null
    // everything inbetween are our actual args
    std::vector<char*>* envs = nullptr;
    auto args = new std::vector<char*>(2 + hxArgs->length);
    args->at(0) = const_cast<char*>(_file.utf8_str());
    args->at(1 + hxArgs->length) = nullptr;

    for (auto i = 0; i < hxArgs->length; i++)
    {
        args->at(1 + i) = const_cast<char*>(hxArgs[i].utf8_str());
    }

    auto options = new uv_process_options_t();
    options->file        = args->at(0);
    options->args        = args->data();
    options->stdio       = stdio->data();
    options->stdio_count = stdio->size();
    options->exit_cb     = [](uv_process_t* process, int64_t status, int signal) {
        delete static_cast<SpawnData*>(process->data);
        delete process;
    };

    if (null() != hxCwd)
    {
        options->cwd = hxCwd.utf8_str();
    }
    if (nullptr != hxEnv)
    {
        options->env = (envs = createLuvEnvironmentVars(hxEnv))->data();
    }

    auto data    = new SpawnData(args, envs, options, _success.mPtr, _failure.mPtr, stdinPipe, stdoutPipe, stderrPipe);
    auto request = new uv_process_t();
    request->data = data;

    auto response = 0;
    if ((response = uv_spawn(_loop, request, options)) < 0)
    {
        delete data;
        delete request;

        _failure(response);
    }
    else
    {
        _success(cpp::Pointer<uv_process_t>(request));
    }
}

uv_stream_t* cpp::luv::process::getStdioStream(uv_process_t* _process, int _index)
{
    auto data = static_cast<SpawnData*>(_process->data);

    switch (_index)
    {
        case 0:
            return reinterpret_cast<uv_stream_t*>(data->stdinPipe);

        case 1:
            return reinterpret_cast<uv_stream_t*>(data->stdoutPipe);

        case 2:
            return reinterpret_cast<uv_stream_t*>(data->stderrPipe);

        default:
            return nullptr;
    }
}