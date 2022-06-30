package cpp.asio;

import haxe.ds.Option;
import cpp.luv.streams.DuplexStream;
import cpp.luv.Luv.LuvTcp;
import sys.thread.Thread;
import cpp.asio.streams.IDuplexStream;

class TcpSocket
{
    final server : LuvTcp;

    function new(_server)
    {
        server = _server;
    }

    public static function bind(_address : String, _port : Int, _result : Result<TcpSocket, Code>->Void)
    {
        cpp.luv.TcpSocket.bind(
            @:privateAccess Thread.current().events.luvLoop,
            _address,
            _port,
            server -> _result(Result.Success(new TcpSocket(server))),
            code -> _result(Result.Error(new Code(code))));
    }

    public function listen(_result : Result<TcpRequest, Code>->Void)
    {
        cpp.luv.TcpSocket.listen(
            server,
            result -> {
                if (result < 0)
                {
                    _result(Result.Error(new Code(result)));
                }
                else
                {
                    _result(Result.Success(new TcpRequest(server)));
                }
            });
    }

    public function shutdown(_result : Option<Code>->Void)
    {
        cpp.luv.Stream.shutdown(
            cpp.luv.TcpSocket.asStream(server),
            code -> {
                if (code < 0)
                {
                    _result(Option.Some(new Code(code)));
                }
                else
                {
                    _result(Option.None);
                }
            });
    }

    public function close()
    {
        cpp.luv.Stream.close(cpp.luv.TcpSocket.asStream(server));
    }
}

class TcpRequest
{
    final server : LuvTcp;

    public function new(_server)
    {
        server = _server;
    }

    public function accept(_result : Result<TcpClient, Code>->Void)
    {
        cpp.luv.TcpSocket.accept(
            server,
            client -> _result(Result.Success(new TcpClient(client))),
            code -> _result(Result.Error(code)));
    }

    public function reject()
    {
        //
    }
}

class TcpClient
{
    final client : LuvTcp;

    public final stream : IDuplexStream;

    public function new(_client)
    {
        client = _client;
        stream = new DuplexStream(cpp.luv.TcpSocket.asStream(client));
    }

    public function shutdown(_result : Option<Code>->Void)
    {
        cpp.luv.Stream.shutdown(
            cpp.luv.TcpSocket.asStream(client),
            result -> {
                if (result < 0)
                {
                    _result(Option.Some(new Code(result)));
                }
                else
                {
                    _result(Option.None);
                }
            });
    }

    public function close()
    {
        cpp.luv.Stream.close(cpp.luv.TcpSocket.asStream(client));
    }
}