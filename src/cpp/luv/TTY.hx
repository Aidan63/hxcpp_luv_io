package cpp.luv;

import cpp.luv.Luv.LuvStream;
import cpp.asio.Code;
import cpp.luv.Luv.LuvTTY;
import cpp.luv.Luv.LuvLoop;

@:keep
@:unreflective
@:include('HxcppLuvTTY.hpp')
extern class TTY
{
    @:native('cpp::luv::tty::init')
    static function init(_loop : LuvLoop, _type : Int, _success : LuvTTY->Void, _failure : Code->Void) : Void;

    @:native('cpp::luv::tty::close')
    static function close(handle : LuvTTY) : Void;

    @:native('cpp::luv::tty::asStream')
    static function asStream(handle : LuvTTY) : LuvStream;
}