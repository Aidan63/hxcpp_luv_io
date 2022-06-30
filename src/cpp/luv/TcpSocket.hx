package cpp.luv;

import cpp.luv.Luv.LuvStream;
import cpp.asio.Code;
import cpp.luv.Luv.LuvTcp;
import cpp.luv.Luv.LuvLoop;

@:keep
@:unreflective
@:include("HxcppLuvTcp.hpp")
extern class TcpSocket
{
    @:native('::cpp::luv::tcp::bind')
    static function bind(_loop : LuvLoop, _address : String, _port : Int, _success : LuvTcp->Void, _failure : Code->Void) : Void;

    @:native('::cpp::luv::tcp::listen')
    static function listen(_server : LuvTcp, _callback : Int->Void) : Void;

    @:native('::cpp::luv::tcp::accept')
    static function accept(_server : LuvTcp, _success : LuvTcp->Void, _failure : Code->Void) : Void;

    @:native('::cpp::luv::tcp::asStream')
    static function asStream(_server: LuvTcp) : LuvStream;
}