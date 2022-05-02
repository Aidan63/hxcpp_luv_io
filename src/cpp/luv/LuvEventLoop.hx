package cpp.luv;

import cpp.luv.Luv.LuvLoop;
import cpp.luv.Luv.LuvHandle;

@:keep
@:unreflective
@:include('HxcppLuv.hpp')
extern class LuvEventLoop
{
    @:native('cpp::luv::queue_repeat_task')
    static function queueRepeatTask(loop : LuvLoop, interval : Int, callback : Void->Void) : LuvHandle;

    @:native('cpp::luv::cancel_task')
    static function cancelTask(handle : LuvHandle) : Bool;
}