package cpp.asio;

import sys.thread.Thread;
import cpp.luv.Luv.LuvSignal;

class Signal
{
    final handle : LuvSignal;

    function new(_handle)
    {
        handle = _handle;
    }

    public function start(_signal : SignalType, _result : Result<SignalType, Code>->Void)
    {
        cpp.luv.Signal.start(
            handle,
            _signal,
            result -> {
                if (result < 0)
                {
                    _result(Result.Error(new Code(result)));
                }
                else
                {
                    _result(Result.Success(cast result));
                }
            });
    }

    public function stop()
    {
        cpp.luv.Signal.stop(handle);
    }

    public function close()
    {
        cpp.luv.Signal.close(handle);
    }

    public static function open(_result : Result<Signal, Code>->Void)
    {
        cpp.luv.Signal.init(
            @:privateAccess Thread.current().events.luvLoop,
            signal -> _result(Result.Success(new Signal(signal))),
            code -> _result(Result.Error(code)));
    }
}