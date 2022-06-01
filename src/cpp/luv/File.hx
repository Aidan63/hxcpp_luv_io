package cpp.luv;

import cpp.asio.Code;
import haxe.io.BytesData;
import cpp.luv.Luv.LuvLoop;

@:keep
@:unreflective
@:include('HxcppLuvFile.hpp')
extern class File
{
    @:native('cpp::luv::file::open')
    static function open(_loop : LuvLoop, _file : String, _flags : Int, _mode : Int, _callback : Int->Void) : Void;

    @:native('cpp::luv::file::read')
    static function read(_loop : LuvLoop, _file : Int, _offset : Int, _success : BytesData->Void, _fail : Int->Void) : Void;

    @:native('cpp::luv::file::write')
    static function write(_loop : LuvLoop, _file : Int, _data : BytesData, _offset : Int, _length : Int, _fileOffset : Int, _callback : Code->Void) : Void;

    @:native('cpp::luv::file::close')
    static function close(_loop : LuvLoop, _file : Int, _callback : Int->Void) : Void;
}