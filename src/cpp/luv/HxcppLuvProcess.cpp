#include <hxcpp.h>
#include "HxcppLuvProcess.hpp"
#include "HxcppLuvStream.hpp"
#include "hx/StdLibs.h"
#include "RootedObject.hpp"
#include "AutoGCZone.hpp"
#include <string>
#include <vector>
#include <array>

namespace cpp::luv::process
{
    class SpawnData
    {
    public:
        const std::unique_ptr<std::vector<char*>> args;
        const std::unique_ptr<std::vector<char*>> envs;
        const std::unique_ptr<uv_process_options_t> options;
        const cpp::utils::RootedObject cbResult;
        uv_pipe_t* const stdinPipe;
        uv_pipe_t* const stdoutPipe;
        uv_pipe_t* const stderrPipe;

        SpawnData(
            std::vector<char*>* _args,
            std::vector<char*>* _envs,
            uv_process_options_t* _options,
            uv_pipe_t* _stdinPipe,
            uv_pipe_t* _stdoutPipe,
            uv_pipe_t* _stderrPipe)
            : args(_args)
            , options(_options)
            , envs(_envs)
            , cbResult(cpp::utils::RootedObject())
            , stdinPipe(_stdinPipe)
            , stdoutPipe(_stdoutPipe)
            , stderrPipe(_stderrPipe)
        {
            //
        }

        ~SpawnData()
        {
            auto callback = [](uv_handle_t* handle) {
                delete static_cast<cpp::luv::stream::StreamData*>(handle->data);
                delete handle;
            };

            uv_close(reinterpret_cast<uv_handle_t*>(stdinPipe), callback);
            uv_close(reinterpret_cast<uv_handle_t*>(stdoutPipe), callback);
            uv_close(reinterpret_cast<uv_handle_t*>(stderrPipe), callback);
        }
    };

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

    String getCwd(hx::Anon _options)
    {
        if (null() == _options)
        {
            return null();
        }

        auto field = _options->__Field("cwd", HX_PROP_ALWAYS);

        if (field.isNull())
        {
            return null();
        }

        return field.asString();
    }

    Dynamic getEnv(hx::Anon _options)
    {
        if (null() == _options)
        {
            return null();
        }

        auto field = _options->__Field("env", HX_PROP_ALWAYS);

        if (field.isNull())
        {
            return null();
        }

        return Dynamic(field.asObject()->__Field("h", HX_PROP_ALWAYS).asObject());
    }

    Array<String> getArgs(hx::Anon _options)
    {
        if (null() == _options)
        {
            return Array<String>(0, 0);
        }

        auto field = _options->__Field("args", HX_PROP_ALWAYS);

        if (field.isNull())
        {
            return Array<String>(0, 0);
        }

        return Array<String>(RawAutoCast(field.asObject()));
    }
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

    auto hxCwd  = getCwd(_options);
    auto hxArgs = getArgs(_options);
    auto hxEnv  = getEnv(_options);

    // arg 0 must be the program, and the final one must be null
    // everything inbetween are our actual args
    std::vector<char*>* envs = nullptr;
    std::vector<char*>* args = nullptr;

    if (_options->__Field("shell", HX_PROP_ALWAYS).asInt())
    {
        args = new std::vector<char*>(4);

#ifdef HX_WINDOWS
        auto cmdexe = getenv("COMSPEC");

        args->at(0) = cmdexe ? cmdexe : "cmd.exe";
        args->at(1) = "/C";
#else
        args->at(0) = "/bin/sh";
        args->at(1) = "-c";
#endif

        hxArgs->insert(0, _file);

        args->at(2) = const_cast<char*>(hxArgs->join(HX_CSTRING(" ")).utf8_str());
        args->at(3) = nullptr;

        printf("%s %s %s\n", args->at(0), args->at(1), args->at(2));
    }
    else
    {
        args = new std::vector<char*>(2 + hxArgs->length);
        args->at(0) = const_cast<char*>(_file.utf8_str());
        args->at(1 + hxArgs->length) = nullptr;

        for (auto i = 0; i < hxArgs->length; i++)
        {
            args->at(1 + i) = const_cast<char*>(hxArgs[i].utf8_str());
        }
    }

    auto wrapper = [](uv_process_t* proc, int64_t status, int signal) {
        auto gcZone = cpp::utils::AutoGCZone();
        auto spProc = std::unique_ptr<uv_process_t>{ proc };
        auto spData = std::unique_ptr<SpawnData>{ static_cast<SpawnData*>(proc->data) };
        auto sp     = Dynamic(spData->cbResult.getObject());

        sp(status, signal);
    };

    auto options = new uv_process_options_t();
    options->file        = args->at(0);
    options->args        = args->data();
    options->stdio       = stdio->data();
    options->stdio_count = stdio->size();
    options->exit_cb     = wrapper;

    if (null() != hxCwd)
    {
        options->cwd = hxCwd.utf8_str();
    }
    if (null() != hxEnv)
    {
        options->env = (envs = createLuvEnvironmentVars(hxEnv))->data();
    }

    auto request  = new uv_process_t();
    auto response = 0;

    if ((response = uv_spawn(_loop, request, options)) < 0)
    {
        _failure(response);
    }
    else
    {
        request->data = new SpawnData(args, envs, options, stdinPipe, stdoutPipe, stderrPipe);
        request->exit_cb = wrapper;

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

void cpp::luv::process::setExitCallback(uv_process_t* _process, Dynamic _cbResult)
{
    auto data = static_cast<SpawnData*>(_process->data);

    data->cbResult.setObject(_cbResult.mPtr);
}