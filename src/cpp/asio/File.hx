package cpp.asio;

import haxe.io.Encoding;
import haxe.io.ArrayBufferView;
import haxe.io.Output;
import haxe.io.Input;
import haxe.ds.Option;
import sys.thread.Thread;
import haxe.io.Bytes;

class File
{
    final file : Int;

    final input : Input;

    final output : Output;

    function new(_file)
    {
        file   = _file;
        input  = null;
        output = null;
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
    public function writeBytes(_bytes : Bytes, _callback : Result<Int, Code>->Void = null)
    {
        cpp.luv.File.write(
            @:privateAccess Thread.current().events.luvLoop,
            file,
            _bytes.getData(),
            0,
            _bytes.length,
            makeWriteCallback(_callback)
        );
    }

    /**
     * Write the provided string to the file.
     * 
     * @param _string String to write to the file.
     * 
     * @param _callback Callback function to report the write result.
     * In the case of a success the integer is the number of bytes written to the file.
     * For failures the code contains the reason.
     */
    public function writeString(_string : String, _callback : Result<Int, Code>->Void)
    {
        writeBytes(Bytes.ofString(_string), _callback);
    }

    /**
     * Write the provided buffer view to the file.
     * 
     * @param _buffer Buffer view too write too the file.
     * Modifying the bytes object while the write is in progress is undefined behaviour.
     * 
     * @param _callback Callback function to report the write result.
     * In the case of a success the integer is the number of bytes written to the file.
     * For failures the code contains the reason.
     */
    public function writeArrayBufferView(_buffer : ArrayBufferView, _callback : Result<Int, Code>->Void)
    {
        cpp.luv.File.write(
            @:privateAccess Thread.current().events.luvLoop,
            file,
            _buffer.buffer.getData(),
            _buffer.byteOffset,
            _buffer.byteLength,
            makeWriteCallback(_callback));
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

    public function readFrom(_offset : Int, _callback : Result<Bytes, Code>->Void)
    {
        cpp.luv.File.read(
            @:privateAccess Thread.current().events.luvLoop,
            file,
            _offset,
            data -> _callback(Result.Success(Bytes.ofData(data))),
            code -> _callback(Result.Error(new Code(code))));
    }

    public function readInto(_buffer : ArrayBufferView, _callback : Result<Int, Code>->Void)
    {
        //
    }

    public function readIntoFrom(_buffer : ArrayBufferView, _offset : Int, _callback : Result<Int, Code>->Void)
    {
        //
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

    static function makeWriteCallback(_callback : Result<Int, Code>->Void)
    {
        return
            (code : Code) -> {
                if (code.isError())
                {
                    _callback(Result.Error(code));
                }
                else
                {
                    _callback(Result.Success(code));
                }
            }
    }
}