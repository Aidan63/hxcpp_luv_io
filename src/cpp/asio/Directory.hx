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

    /**
     * Read a single entry from the directory.
     * 
     * If `Result.Error` returns `Code.eof` then there are no more entries to read.
     * @param _callback Callback to handle the response.
     */
    public function read(_callback : Result<Entry, Code>->Void)
    {
        cpp.luv.Directory.read(
            @:privateAccess Thread.current().events.luvLoop,
            dir,
            dirent -> _callback(Result.Success(new Entry(cast dirent.ptr.type, dirent.ptr.name))),
            code -> _callback(Result.Error(code)));
    }

    /**
     * Iterate over all entries in the directory.
     * 
     * The callback with the `Result.Success` option is called for each entry found.
     * After `Result.Error` is returned no more entries will be read.
     * If `Result.Error` returns `Code.eof` then there are no more entries to read.
     * @param _callback Callback to handle the response.
     */
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

    /**
     * Close the directory
     * No more `read` or `iter` calls can be made after closing, any pending requests will also be cancelled.
     * @param _callback Callback to handle the response. `Some(Code)` is returned on an error.
     */
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

    /**
     * Open a directory for reading.
     * @param _path Path to the directory.
     * @param _callback Callback to handle the response.
     */
    public static function open(_path : String, _callback : Result<Directory, Code>->Void)
    {
        cpp.luv.Directory.open(
            @:privateAccess Thread.current().events.luvLoop,
            _path,
            dir -> _callback(Result.Success(new Directory(dir))),
            code -> _callback(Result.Error(code)));
    }
}