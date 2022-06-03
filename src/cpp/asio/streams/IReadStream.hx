package cpp.asio.streams;

import haxe.io.Bytes;

interface IReadStream
{
    function read(_callback : Result<Bytes, Code>->Void) : Void;

    function stop() : Void;
}