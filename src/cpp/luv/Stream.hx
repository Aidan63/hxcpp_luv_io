package cpp.luv;

import cpp.luv.Luv.LuvStream;

@:keep
@:unreflective
@:include('HxcppLuvStream.hpp')
extern class Stream
{
    @:native('::cpp::luv::write')
    static function write(_stream : LuvStream, _data : Array<cpp.UInt8>, _offset : Int, _length : Int, _callback : Int->Void) : Void;
}