package cpp.luv;

import cpp.asio.Code;
import cpp.luv.Luv.LuvLoop;
import cpp.luv.Luv.LuvSignal;

@:keep
@:unreflective
@:include('HxcppLuvSignal.hpp')
extern class Signal
{
    @:native('cpp::luv::signal::init')
    static function init(_loop : LuvLoop, _success : LuvSignal->Void, _failure : Code->Void) : Void;

    @:native('cpp::luv::signal::start')
    static function start(_signal : LuvSignal, _signnum : Int, _result : Int->Void) : Void;

    @:native('cpp::luv::signal::stop')
    static function stop(_signal : LuvSignal) : Void;

    @:native('cpp::luv::signal::close')
    static function close(_signal : LuvSignal) : Void;
}