package cpp.luv;

import cpp.luv.Luv.LuvLoop;
import cpp.luv.Luv.LuvAsync;

@:keep
@:unreflective
@:include('HxcppLuvAsync.hpp')
extern class Async
{
    @:native('::cpp::luv::async::init')
    static function init(_loop : LuvLoop, _callback : Void->Void) : LuvAsync;

    @:native('::cpp::luv::async::send')
    static function send(_async : LuvAsync) : Int;

    @:native('::cpp::luv::async::close')
    static function close(_async : LuvAsync) : Int;

    @:native('::cpp::luv::async::unref')
    static function unref(_async : LuvAsync) : Void;
}