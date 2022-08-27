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

@:keep
@:unreflective
@:include('uv.h')
@:native('::cpp::Pointer<uv_timer_t>')
extern class LuvTimer {}

@:keep
@:unreflective
@:include('uv.h')
@:native('::cpp::Pointer<uv_stream_t>')
extern class LuvStream {}

@:keep
@:unreflective
@:include('uv.h')
@:native('::cpp::Pointer<uv_dir_t>')
extern class LuvDir {}

@:keep
@:unreflective
@:include('uv.h')
@:native('::cpp::Pointer<uv_process_t>')
extern class LuvProcess {}

@:keep
@:unreflective
@:include('uv.h')
@:native('::cpp::Pointer<uv_async_t>')
extern class LuvAsync {}

@:keep
@:unreflective
@:include('uv.h')
@:native('::cpp::Pointer<uv_tty_t>')
extern class LuvTTY {}

@:keep
@:unreflective
@:include('uv.h')
@:native('::cpp::Pointer<uv_signal_t>')
extern class LuvSignal {}

@:keep
@:unreflective
@:include('uv.h')
@:native('::cpp::Pointer<uv_tcp_t>')
extern class LuvTcp {}

enum abstract RunMode(Int) {
    var Default;
    var Once;
    var NoWait;
}

@:keep
@:unreflective
@:include('HxcppLuv.hpp')
@:build(linc.Linc.touch())
@:build(linc.Linc.xml('Luv'))
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