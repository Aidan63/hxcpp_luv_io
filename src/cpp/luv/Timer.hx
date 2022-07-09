package cpp.luv;

import cpp.luv.Luv.LuvTimer;
import cpp.luv.Luv.LuvLoop;

@:keep
@:unreflective
@:include('HxcppLuvTimer.hpp')
extern class Timer
{
    @:native('cpp::luv::timer::repeat')
    static function repeat(loop : LuvLoop, interval : Int, callback : Void->Void) : LuvTimer;

    @:native('cpp::luv::timer::run')
    static function run(loop : LuvLoop, callback : Void->Void) : LuvTimer;

    @:native('cpp::luv::timer::stop')
    static function stop(handle : LuvTimer) : Int;

    @:native('cpp::luv::timer::close')
    static function close(handle : LuvTimer) : Void;
}