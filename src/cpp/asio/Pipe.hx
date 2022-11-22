package cpp.asio;

import haxe.ds.Option;
import sys.thread.Thread;
import cpp.luv.streams.WriteStream;
import cpp.luv.streams.ReadStream;
import cpp.asio.streams.IWriteStream;
import cpp.asio.streams.IReadStream;
import cpp.luv.Luv.LuvPipe;

class Pipe
{
    final handle : LuvPipe;

    public final read : IReadStream;

    public final write : IWriteStream;

    function new(_handle)
    {
        handle = _handle;
        read   = new ReadStream(cpp.luv.Pipe.asStream(handle));
        write  = new WriteStream(cpp.luv.Pipe.asStream(handle));
    }

    public static function open(_file : Int, _callback : Result<Pipe, Code>->Void)
    {
        cpp.luv.Pipe.open(
            @:privateAccess Thread.current().events.luvLoop,
            _file,
            pipe -> _callback(Result.Success(new Pipe(pipe))),
            code -> _callback(Result.Error(code)));
    }
}