#include "hxcpp.h"
#include "HxcppLuvTcp.hpp"
#include "HxcppLuvStream.hpp"
#include "AutoGCZone.hpp"
#include "RootedObject.hpp"
#include <memory>

void cpp::luv::tcp::bind(uv_loop_t* loop, String address, int port, Dynamic success, Dynamic failure)
{
    auto result = 0;

    auto socket = sockaddr_in();
    if ((result = uv_ip4_addr(address.utf8_str(), port, &socket)) < 0)
    {
        failure(result);

        return;
    }

    auto server = std::make_unique<uv_tcp_t>();
    if ((result = uv_tcp_init(loop, server.get())) < 0)
    {
        failure(result);

        return;
    }

    if ((result = uv_tcp_bind(server.get(), reinterpret_cast<const sockaddr*>(&socket), 0)) < 0)
    {
        uv_close(reinterpret_cast<uv_handle_t*>(server.release()), [](uv_handle_t* handle) { delete handle; });

        failure(result);
    }
    else
    {
        success(cpp::Pointer<uv_tcp_t>(server.release()));
    }
}

void cpp::luv::tcp::listen(uv_tcp_t* server, Dynamic callback)
{
    auto result = 0;

    auto wrapper = [](uv_stream_t* server, int status) {
        auto gcZone   = cpp::utils::AutoGCZone();
        auto rooted   = static_cast<hx::Object**>(server->data);
        auto callback = Dynamic(*rooted);

        callback(status);
    };

    if ((result = uv_listen(reinterpret_cast<uv_stream_t*>(server), 128, wrapper)) < 0)
    {
        callback(result);
    }
    else
    {
        auto root = new hx::Object*(callback.mPtr);

        hx::GCAddRoot(root);

        server->data = root;
    }
}

void cpp::luv::tcp::accept(uv_tcp_t* server, Dynamic success, Dynamic failure)
{
    auto result = 0;
    auto client = std::make_unique<uv_tcp_t>();

    uv_tcp_init(server->loop, client.get());

    if ((result = uv_accept(reinterpret_cast<uv_stream_t*>(server), reinterpret_cast<uv_stream_t*>(client.get()))) < 0)
    {
        failure(result);
    }
    else
    {
        client->data = new cpp::luv::stream::StreamData();

        success(cpp::Pointer<uv_tcp_t>(client.release()));
    }
}

uv_stream_t* cpp::luv::tcp::asStream(uv_tcp_t* client)
{
    return reinterpret_cast<uv_stream_t*>(client);
}