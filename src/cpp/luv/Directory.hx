package cpp.luv;

import cpp.asio.Code;
import cpp.luv.Luv.LuvDir;
import cpp.luv.Luv.LuvLoop;

@:keep
@:unreflective
@:include('HxcppLuvDirectory.hpp')
extern class Directory
{
    @:native('cpp::luv::directory::open')
    static function open(_loop : LuvLoop, _path : String, _success : LuvDir->Void, _failure : Code->Void) : Void;

    @:native('cpp::luv::directory::read')
    static function read(_loop : LuvLoop, _dir : LuvDir, _success : Dirent->Void, _failure : Code->Void) : Void;

    @:native('cpp::luv::directory::close')
    static function close(_loop : LuvLoop, _dir : LuvDir, _callback : Code->Void) : Void;
}

@:keep
@:unreflective
@:include('uv.h')
@:native('uv_dirent_t')
private extern class Dirent_t
{
    final type : Int;

    final name : cpp.ConstCharStar;
}

private typedef Dirent = cpp.Pointer<Dirent_t>;