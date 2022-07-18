package cpp.asio;

import cpp.luv.streams.ReadStream;
import cpp.luv.Luv.LuvProcess;
import sys.thread.Thread;
import cpp.asio.streams.IReadStream;
import cpp.asio.streams.IWriteStream;

class Process
{
    final proc : LuvProcess;

    public final stdin : IWriteStream;

    public final stdout : IReadStream;

    public final stderr : IReadStream;

    public var onExit : Int->Int->Void;

    function new(_proc)
    {
        proc   = _proc;
        stdin  = null;
        stdout = new ReadStream(cpp.luv.Process.getStdioStream(proc, 1));
        stderr = new ReadStream(cpp.luv.Process.getStdioStream(proc, 2));
        onExit = null;

        cpp.luv.Process.setExitCallback(proc, onExitForward);
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

    function onExitForward(_status : Int, _signal : Int)
    {
        if (onExit != null)
        {
            onExit(_status, _signal);
        }
    }
}