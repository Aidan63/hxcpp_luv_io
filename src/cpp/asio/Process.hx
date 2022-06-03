package cpp.asio;

import cpp.luv.Luv.LuvProcess;
import sys.thread.Thread;
import cpp.asio.streams.IReadStream;
import cpp.asio.streams.IWriteStream;

typedef ProcessOptions = {
    var args : Array<String>;
    var env : Map<String, String>;
    var cwd : String;
}

class Process
{
    final proc : LuvProcess;

    public final stdin : IWriteStream;

    public final stdout : IReadStream;

    public final stderr : IReadStream;

    function new(_proc)
    {
        proc   = _proc;
        stdin  = null;
        stdout = null;
        stderr = null;
    }

    public static function spawn(_file : String, _options : ProcessOptions, _callback : Result<Process, Code>->Void)
    {
        cpp.luv.Process.spawn(
            @:privateAccess Thread.current().events.luvLoop,
            _file,
            _options,
            proc -> _callback(Result.Success(new Process(proc))),
            code -> _callback(Result.Error(code)));
    }
}