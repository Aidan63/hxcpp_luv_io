package cpp.luv;

import cpp.luv.Luv.LuvTimer;
import cpp.luv.Luv.LuvLoop;

@:keep
@:unreflective
@:include('HxcppLuv.hpp')
extern class LuvEventLoop
{
    @:native('cpp::luv::queue_repeat_task')
    static function queueRepeatTask(loop : LuvLoop, interval : Int, callback : Void->Void) : LuvTimer;

    @:native('cpp::luv::timer_stop')
    static function timer_stop(handle : LuvTimer) : Int;

    @:native('cpp::luv::timer_close')
    static function timer_close(handle : LuvTimer) : Void;
}