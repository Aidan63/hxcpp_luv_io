package cpp.luv;

@:keep
@:unreflective
@:include('uv.h')
@:native('::cpp::Pointer<uv_loop_t>')
extern class LuvLoop {}

@:keep
@:unreflective
@:include('uv.h')
@:native('::cpp::Pointer<uv_handle_t>')
extern class LuvHandle {}

enum abstract RunMode(Int) {
    var Default;
    var Once;
    var NoWait;
}

@:keep
@:unreflective
@:include('HxcppLuv.hpp')
@:buildXml("
<compiler id='MSVC' exe='cl.exe'>
    <flag value = '/std:c++17'/>
</compiler>
<files id='haxe'>
    <compilerflag value='-IC:/Users/AidanLee/Desktop/hxcpp_luv/src/cpp/luv'/>
    <compilerflag value='-IC:/Users/AidanLee/Desktop/hxcpp_luv/src/cpp/utils'/>

    <file name='C:/Users/AidanLee/Desktop/hxcpp_luv/src/cpp/luv/HxcppLuv.cpp'/>
    <file name='C:/Users/AidanLee/Desktop/hxcpp_luv/src/cpp/luv/HxcppLuvFile.cpp'/>
    <file name='C:/Users/AidanLee/Desktop/hxcpp_luv/src/cpp/utils/AutoGCZone.cpp'/>
</files>

<files id='luv'>
    <compilerflag value='-IC:/Users/AidanLee/Desktop/hxcpp_luv/src/cpp/luv'/>
    <compilerflag value='-IC:/Users/AidanLee/Desktop/hxcpp_luv/src/cpp/utils'/>
</files>

<target id='haxe'>
    <files id='luv'/>
</target>")
extern class Luv
{
    @:native('::cpp::luv::allocLoop')
    static function allocLoop() : LuvLoop;

    @:native('::cpp::luv::freeLoop')
    static function freeLoop(_loop : LuvLoop) : Void;

    @:native('::cpp::luv::runLoop')
    static function runLoop(_loop : LuvLoop, _mode : RunMode) : Bool;

    @:native('::cpp::luv::errorToString')
    static function errorToString(_error : Int) : String;
}