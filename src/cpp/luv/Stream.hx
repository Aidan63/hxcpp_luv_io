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

    @:native('cpp::luv::stream::write')
    static function write(_stream : LuvStream, _buffer : BytesData, _callback : Int->Void) : Void;

    @:native('cpp::luv::stream::shutdown')
    static function shutdown(_stream : LuvStream, _callback : Int->Void) : Void;

    @:native('cpp::luv::stream::close')
    static function close(_stream : LuvStream) : Void;
}