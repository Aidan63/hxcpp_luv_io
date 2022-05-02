package cpp.luv;

import sys.thread.Thread;
import cpp.luv.Luv.LuvLoop;

@:keep
@:unreflective
@:include('HxcppLuv.hpp')
extern class Process
{
    static inline function spawn(_file : String, _args : Array<String>, _directory : String, _stdout : String->Void, _stderr : String->Void, _complete : Int->Void)
    {
        luvSpawn(@:privateAccess Thread.current().events.luvLoop, _file, _args, _directory, _stdout, _stderr, _complete);
    }

    @:native('cpp::luv::luvSpawn')
    private static function luvSpawn(_loop : LuvLoop, _file : String, _args : Array<String>, _directory : String, _stdout : Dynamic, _stderr : Dynamic, _complete : Dynamic) : Void;
}