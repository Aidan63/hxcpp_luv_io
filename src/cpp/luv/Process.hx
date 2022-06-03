package cpp.luv;

import cpp.luv.Luv.LuvStream;
import cpp.asio.Code;
import cpp.luv.Luv.LuvLoop;
import cpp.luv.Luv.LuvProcess;

@:keep
@:unreflective
@:include('HxcppLuvProcess.hpp')
extern class Process
{
    @:native('cpp::luv::process::spawn')
    static function spawn(_loop : LuvLoop, _file : String, _options : Object, _success : LuvProcess->Void, _failure : Code->Void) : Void;

    @:native('cpp::luv::process::getStdioStream')
    static function getStdioStream(_process : LuvProcess, _index : Int) : LuvStream;
}