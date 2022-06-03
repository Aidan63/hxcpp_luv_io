package cpp.luv.streams;

import cpp.asio.streams.IReadStream;
import cpp.asio.Code;
import cpp.asio.Result;
import haxe.io.Bytes;
import cpp.luv.Luv.LuvStream;

class ReadStream implements IReadStream
{
    final stream : LuvStream;

    public function new(_stream)
    {
        stream = _stream;
    }

	public function read(_callback : Result<Bytes, Code>->Void)
    {
        cpp.luv.Stream.read(
            stream,
            data -> _callback(Result.Success(Bytes.ofData(data))),
            code -> _callback(Result.Error(code)));
    }

	public function stop()
    {
        cpp.luv.Stream.stop(stream);
    }
}