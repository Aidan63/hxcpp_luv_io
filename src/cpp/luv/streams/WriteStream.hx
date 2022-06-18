package cpp.luv.streams;

import cpp.asio.streams.IWriteStream;
import cpp.asio.Code;
import haxe.io.Bytes;
import haxe.ds.Option;
import cpp.luv.Luv.LuvStream;

class WriteStream implements IWriteStream
{
    final stream : LuvStream;

    public function new(_stream)
    {
        stream = _stream;
    }

    public function write(_bytes : Bytes, _callback : Option<Code>->Void)
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
}