package cpp.asio.streams;

import haxe.ds.Option;
import haxe.io.Bytes;

interface IWriteStream
{
    function write(_bytes : Bytes, _callback : Option<Code>->Void) : Void;

    function shutdown(_callback : Option<Code>->Void) : Void;
}