package cpp.luv.streams;

import cpp.luv.Luv.LuvStream;
import cpp.asio.Result;
import haxe.io.Bytes;
import cpp.asio.Code;
import haxe.ds.Option;
import cpp.asio.streams.IDuplexStream;

class DuplexStream implements IDuplexStream
{
    final stream : LuvStream;

    public function new(_stream)
    {
        stream = _stream;
    }

	public function read(_callback:Result<Bytes, Code> -> Void)
    {
        cpp.luv.Stream.read(
            stream,
            data -> _callback(Result.Success(Bytes.ofData(data))),
            code -> _callback(Result.Error(code)));
    }

	public function write(_bytes:Bytes, _callback:Option<Code> -> Void)
    {
        cpp.luv.Stream.write(
            stream,
            _bytes.getData(),
            status -> {
                if (status < 0)
                {
                    _callback(Option.Some(new Code(status)));
                }
                else
                {
                    _callback(Option.None);
                }
            });
    }

	public function stop()
    {
        cpp.luv.Stream.stop(stream);
    }

    public function shutdown(_callback : Option<Code>->Void)
    {
        cpp.luv.Stream.shutdown(
            stream,
            result -> {
                if (result < 0)
                {
                    _callback(Option.Some(new Code(result)));
                }
                else
                {
                    _callback(Option.None);
                }
            });
    }
}