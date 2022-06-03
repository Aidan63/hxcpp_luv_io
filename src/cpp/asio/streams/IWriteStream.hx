package cpp.asio.streams;

import haxe.io.Bytes;

interface IWriteStream
{
    function write(_bytes : Bytes, _callback : Result<Int, Code>) : Void;
}