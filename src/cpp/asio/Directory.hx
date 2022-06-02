package cpp.asio;

import sys.thread.Thread;
import cpp.luv.Luv.LuvDir;
import haxe.ds.Option;

class Directory
{
    final dir : LuvDir;

    function new(_dir)
    {
        dir = _dir;
    }

    public function read(_callback : Result<Entry, Code>->Void)
    {
        cpp.luv.Directory.read(
            @:privateAccess Thread.current().events.luvLoop,
            dir,
            dirent -> _callback(Result.Success(new Entry(cast dirent.ptr.type, dirent.ptr.name))),
            code -> _callback(Result.Error(code)));
    }

    public function iter(_callback : Result<Entry, Code>->Void)
    {
        cpp.luv.Directory.read(
            @:privateAccess Thread.current().events.luvLoop,
            dir,
            dirent -> {
                _callback(Result.Success(new Entry(cast dirent.ptr.type, dirent.ptr.name)));

                iter(_callback);
            },
            code -> _callback(Result.Error(code)));
    }

    public function close(_callback : Option<Code>->Void)
    {
        cpp.luv.Directory.close(
            @:privateAccess Thread.current().events.luvLoop,
            dir,
            code -> {
                if (code.isError())
                {
                    _callback(Option.Some(code));
                }
                else
                {
                    _callback(Option.None);
                }
            }
        );
    }

    public static function open(_directory : String, _callback : Result<Directory, Code>->Void)
    {
        cpp.luv.Directory.open(
            @:privateAccess Thread.current().events.luvLoop,
            _directory,
            dir -> _callback(Result.Success(new Directory(dir))),
            code -> _callback(Result.Error(code)));
    }
}