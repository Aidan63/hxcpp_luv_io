package cpp.luv;

import cpp.asio.Code;
import haxe.io.BytesData;
import cpp.luv.Luv.LuvStream;

@:keep
@:unreflective
@:include("HxcppLuvStream.hpp")
extern class Stream
{
    @:native('cpp::luv::stream::read')
    static function read(_stream : LuvStream, _success : BytesData->Void, _failure : Code->Void) : Void;

    @:native('cpp::luv::stream::stop')
    static function stop(_stream : LuvStream) : Void;
}