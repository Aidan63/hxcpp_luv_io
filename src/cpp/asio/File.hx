package cpp.asio;

import haxe.io.BytesData;
import haxe.ds.Option;
import sys.thread.Thread;
import haxe.io.Bytes;

extern enum abstract OpenMode(Int) to Int
{
    @:native('UV_FS_O_APPEND') var Append;
    @:native('UV_FS_O_CREAT') var Create;
    @:native('UV_FS_O_DIRECT') var Direct;
    @:native('UV_FS_O_DIRECTORY') var Directory;
    @:native('UV_FS_O_DSYNC') var DSync;
    @:native('UV_FS_O_EXCL') var Exclusive;
    @:native('UV_FS_O_EXLOCK') var ExclusiveLock;
    @:native('UV_FS_O_FILEMAP') var FileMap;
    @:native('UV_FS_O_NOATIME') var NoAccessTime;
    @:native('UV_FS_O_NOCTTY') var NoControllingTerminal;
    @:native('UV_FS_O_NOFOLLOW') var NoFollow;
    @:native('UV_FS_O_NONBLOCK') var NonBlocking;
    @:native('UV_FS_O_RANDOM') var Random;
    @:native('UV_FS_O_RDONLY') var ReadOnly;
    @:native('UV_FS_O_RDWR') var ReadWrite;
    @:native('UV_FS_O_SEQUENTIAL') var Sequential;
    @:native('UV_FS_O_SYMLINK') var SymLink;
    @:native('UV_FS_O_SYNC') var Synchronous;
    @:native('UV_FS_O_TEMPORARY') var Temporary;
    @:native('UV_FS_O_TRUNC') var Truncate;
    @:native('UV_FS_O_WRONLY') var WriteOnly;

    @:op(a|b) static function or(a : OpenMode, b : OpenMode) : OpenMode;
    @:op(a&b) static function or(a : OpenMode, b : OpenMode) : OpenMode;
}

class File
{
    final file : Int;

    function new(_file)
    {
        file = _file;
    }

    public inline extern overload function write(_string : String, _callback : Option<Code>->Void)
    {
        write(Bytes.ofString(_string), _callback);
    }

    public inline extern overload function write(_data : Bytes, _callback : Option<Code>->Void)
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

    public static function open(_file : String, _mode : OpenMode, _callback : Result<File, Code>->Void)
    {
        cpp.luv.File.open(
            @:privateAccess Thread.current().events.luvLoop,
            _file,
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