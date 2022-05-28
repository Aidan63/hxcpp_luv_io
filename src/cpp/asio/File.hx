package cpp.asio;

import sys.thread.Thread;
import haxe.ds.Option;
import haxe.io.Bytes;
import haxe.io.ArrayBufferView;

class File
{
    final file : Int;

    final pipe : cpp.luv.Luv.LuvPipe;

    function new(_file)
    {
        file = _file;
        pipe = cpp.luv.Pipe.open(@:privateAccess Thread.current().events.luvLoop, _file);
    }

    public inline extern overload function write2(_buffer : ArrayBufferView)
    {
        //
    }

    public inline extern overload function write2(_buffer : ArrayBufferView, _callback : (_error : Option<Code>)->Void)
    {
        //
    }

    public inline extern overload function write(_string : String, _callback : (_error : Option<Code>)->Void)
    {
        write(Bytes.ofString(_string), _callback);
    }

    public inline extern overload function write(_data : Bytes, _callback : (_error : Option<Code>)->Void)
    {
        cpp.luv.File.write(
            @:privateAccess Thread.current().events.luvLoop,
            file,
            _data.getData(),
            result -> _callback(resultToOptionCode(result)));
    }

    public function read(_callback : Result<Bytes, Code>->Void)
    {
        cpp.luv.File.read(
            @:privateAccess Thread.current().events.luvLoop,
            file,
            data -> _callback(Result.Success(Bytes.ofData(data))),
            code -> _callback(Result.Error(new Code(code))));
    }

    public function close(_callback : Option<Code>->Void)
    {
        cpp.luv.File.close(
            @:privateAccess Thread.current().events.luvLoop,
            file,
            result -> _callback(resultToOptionCode(result)));
    }

    public static function open(_file : String, _flags : OpenMode, _mode : AccessMode, _callback : Result<File, Code>->Void)
    {
        cpp.luv.File.open(
            @:privateAccess Thread.current().events.luvLoop,
            _file,
            _flags,
            _mode,
            result -> {
                if (result >= 0)
                {
                    _callback(Result.Success(new File(new FileHandler(result))));
                }
                else
                {
                    _callback(Result.Error(new Code(result)));
                }
            });
    }

    static function resultToOptionCode(_result)
    {
        return if (_result >= 0)
        {
            Option.None;
        }
        else
        {
            Option.Some(new Code(_result));
        }
    }
}