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

    public function write(_data : Bytes, _callback : Option<Code>->Void)
    {
        //
    }
}