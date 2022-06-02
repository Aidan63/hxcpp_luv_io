#include <hxcpp.h>
#include <memory>
#include "HxcppLuvDirectory.hpp"
#include "RootedObject.hpp"
#include "AutoGCZone.hpp"

void cpp::luv::directory::open(uv_loop_t* _loop, String _path, Dynamic _success, Dynamic _failure)
{
    class OpendirData
    {
    public:
        const cpp::utils::RootedObject cbSuccess;
        const cpp::utils::RootedObject cbFailure;

        OpendirData(hx::Object* _cbSuccess, hx::Object* _cbFailure) :
            cbSuccess(cpp::utils::RootedObject(_cbSuccess)),
            cbFailure(cpp::utils::RootedObject(_cbFailure))
        {
            //
        }
    };

    auto request = new uv_fs_t();
    request->data = new OpendirData(_success.mPtr, _failure.mPtr);

    auto wrapper = [](uv_fs_t* response) {
        auto gcZone     = cpp::utils::AutoGCZone();
        auto spResponse = std::unique_ptr<uv_fs_t>{ response };
        auto request    = std::unique_ptr<OpendirData>{ reinterpret_cast<OpendirData*>(response->data) };

        if (response->result < 0)
        {
            auto callback = Dynamic(request->cbFailure);

            callback(response->result);
        }
        else
        {
            auto dir      = cpp::Pointer<uv_dir_t>(response->ptr);
            auto callback = Dynamic(request->cbSuccess);

            callback(dir);
        }
    };

    auto result = 0;
    if ((result = uv_fs_opendir(_loop, request, _path.utf8_str(), wrapper) < 0))
    {
        delete request->data;
        delete request;

        _failure(result);
    }
}

void cpp::luv::directory::read(uv_loop_t* _loop, uv_dir_t* _dir, Dynamic _success, Dynamic _failure)
{
    class ReaddirData
    {
    public:
        const cpp::utils::RootedObject cbSuccess;
        const cpp::utils::RootedObject cbFailure;
        const std::unique_ptr<uv_dirent_t> entry;

        ReaddirData(hx::Object* _cbSuccess, hx::Object* _cbFailure) :
            cbSuccess(cpp::utils::RootedObject(_cbSuccess)),
            cbFailure(cpp::utils::RootedObject(_cbFailure)),
            entry(std::make_unique<uv_dirent_t>())
        {
            //
        }
    };

    auto data    = new ReaddirData(_success.mPtr, _failure.mPtr);
    auto request = new uv_fs_t();
    request->data = data;

    auto wrapper = [](uv_fs_t* response) {
        auto gcZone     = cpp::utils::AutoGCZone();
        auto spResponse = std::unique_ptr<uv_fs_t>{ response };
        auto request    = std::unique_ptr<ReaddirData>{ reinterpret_cast<ReaddirData*>(response->data) };

        if (response->result < 0)
        {
            auto callback = Dynamic(request->cbFailure);

            callback(response->result);
        }
        else
        {
            auto dir = cpp::Pointer<uv_dirent_t>(request->entry.get());

            if (response->result == 0)
            {
                auto callback = Dynamic(request->cbFailure);

                callback(UV_EOF);
            }
            else
            {
                auto callback = Dynamic(request->cbSuccess);

                callback(dir);
            }

            uv_fs_req_cleanup(response);
        }
    };

    _dir->nentries = 1;
    _dir->dirents  = data->entry.get();

    auto result = 0;
    if ((result = uv_fs_readdir(_loop, request, _dir, wrapper)) < 0)
    {
        delete request->data;
        delete request;

        _failure(result);
    }
}