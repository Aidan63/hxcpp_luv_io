package cpp.asio;

import sys.thread.Thread;
import cpp.asio.streams.IWriteStream;
import cpp.asio.streams.IReadStream;
import cpp.luv.streams.WriteStream;
import cpp.luv.streams.ReadStream;
import cpp.luv.Luv.LuvTTY;

enum abstract Stdio(Int) to Int
{
    var Stdin;
    var Stdout;
    var Stderr;
}

class TTY
{
    final handle : LuvTTY;

    public final read : IReadStream;

    public final write : IWriteStream;

    function new(_handle)
    {
        handle = _handle;
        read   = new ReadStream(cpp.luv.TTY.asStream(handle));
        write  = new WriteStream(cpp.luv.TTY.asStream(handle));
    }

    public function close()
    {
        cpp.luv.TTY.close(handle);
    }

    public static function open(_type : Stdio, _result : Result<TTY, Code>->Void)
    {
        cpp.luv.TTY.init(
            @:privateAccess Thread.current().events.luvLoop,
            _type,
            handle -> _result(Result.Success(new TTY(handle))),
            code -> _result(Result.Error(code)));
    }
}