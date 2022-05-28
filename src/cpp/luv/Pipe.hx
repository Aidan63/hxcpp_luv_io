package cpp.luv;

import cpp.luv.Luv.LuvLoop;
import cpp.luv.Luv.LuvPipe;

@:keep
@:unreflective
@:include('HxcppLuvPipe.hpp')
extern class Pipe
{
    @:native('::cpp::luv::open')
    static function open(_loop : LuvLoop, _file : Int) : LuvPipe;
}