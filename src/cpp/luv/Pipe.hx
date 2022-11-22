package cpp.luv;

import cpp.luv.Luv.LuvStream;
import cpp.luv.Luv.LuvPipe;
import cpp.luv.Luv.LuvLoop;
import cpp.asio.Code;

@:keep
@:unreflective
@:include('HxcppLuvPipe.hpp')
extern class Pipe
{
    @:native('cpp::luv::pipe::open')
    static function open(_pipe : LuvLoop, _handle : Int, _success : LuvPipe->Void, _failure : Code->Void) : Void;

    @:native('cpp::luv::pipe::asStream')
    static function asStream(handle : LuvPipe) : LuvStream;
}