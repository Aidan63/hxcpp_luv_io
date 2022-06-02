package cpp.asio;

import haxe.ds.Option;
import sys.thread.Thread;
import haxe.io.Bytes;

class File
{
    final file : Int;

    function new(_file)
    {
        file = _file;
    }

    /**
     * Write the provided bytes to the file.
     * 
     * @param _bytes Bytes to write to the file.
     * Modifying the bytes object while the write is in progress is undefined behaviour.
     * 
     * @param _callback Callback function to report the write result.
     * In the case of a success the integer is the number of bytes written to the file.
     * For failures the code contains the reason.
     */
    public function write(_bytes : Bytes, _callback : Result<Int, Code>->Void)
    {
        cpp.luv.File.write(
            @:privateAccess Thread.current().events.luvLoop,
            file,
            _bytes.getData(),
            0,
            _bytes.length,
            -1,
            code -> {
                if (code.isError())
                {
                    _callback(Result.Error(code));
                }
                else
                {
                    _callback(Result.Success(code));
                }
            }
        );
    }

    /**
     * Read the entire files contents.
     * 
     * @param _callback Callback function to report the read result.
     */
    public function read(_callback : Result<Bytes, Code>->Void)
    {
        cpp.luv.File.read(
            @:privateAccess Thread.current().events.luvLoop,
            file,
            -1,
            data -> _callback(Result.Success(Bytes.ofData(data))),
            code -> _callback(Result.Error(new Code(code))));
    }

    /**
     * Close the file so no more requests can be made. Any pending requests will also be cancelled.
     * @param _callback Callback to handle the result.
     */
    public function close(_callback : Option<Code>->Void)
    {
        cpp.luv.File.close(
            @:privateAccess Thread.current().events.luvLoop,
            file,
            result -> _callback(resultToOptionCode(result)));
    }

    /**
     * Open a file for reading and / or writing.
     * @param _file Path to the file.
     * @param _flags Flags to specifying which operations are allowed on the opened file.
     * @param _mode Flags for permissions on the opened file.
     * @param _callback Callback to handle the result.
     */
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