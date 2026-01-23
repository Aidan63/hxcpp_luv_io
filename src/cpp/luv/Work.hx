package cpp.luv;

import cpp.luv.Luv.LuvWork;
import cpp.luv.Luv.LuvLoop;

@:keep
@:unreflective
@:include('HxcppLuvWork.hpp')
extern class Work
{
    @:native('cpp::luv::work::queue')
    static function queue(loop : LuvLoop, callback : Void->Void) : LuvWork;
}