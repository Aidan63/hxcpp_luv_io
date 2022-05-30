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
     * If a callback is provided the write is performed asynchronously, other wise this function will not
     * return until the write is complete.
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
     * If a callback is provided the write is performed asynchronously, other wise this function will not
     * return until the write is complete.
     * 
     * @param _string String to write to the file.
     * @param _encoding Encoding to use for the string.
     * @param _callback Callback function to report the write result.
     * In the case of a success the integer is the number of bytes written to the file.
     * For failures the code contains the reason.
     */
    public function writeString(_string : String, _encoding : Encoding = Encoding.UTF8, _callback : Result<Int, Code>->Void = null)
    {
        writeBytes(Bytes.ofString(_string, _encoding), _callback);
    }

    /**
     * Write the provided buffer view to the file.
     * 
     * If a callback is provided the write is performed asynchronously, other wise this function will not
     * return until the write is complete.
     * 
     * @param _buffer Buffer view too write too the file.
     * Modifying the bytes object while the write is in progress is undefined behaviour.
     * 
     * @param _callback Callback function to report the write result.
     * In the case of a success the integer is the number of bytes written to the file.
     * For failures the code contains the reason.
     */
    public function writeArrayBufferView(_buffer : ArrayBufferView, _callback : Result<Int, Code>->Void = null)
    {
        cpp.luv.File.write(
            @:privateAccess Thread.current().events.luvLoop,
            file,
            _buffer.buffer.getData(),
            _buffer.byteOffset,
            _buffer.byteLength,
            makeWriteCallback(_callback)
        );
    }

    // public inline extern overload function write(_string : String, _callback : (_error : Option<Code>)->Void)
    // {
    //     write(Bytes.ofString(_string), _callback);
    // }

    // public inline extern overload function write(_data : Bytes, _callback : (_error : Option<Code>)->Void)
    // {
    //     cpp.luv.File.write(
    //         @:privateAccess Thread.current().events.luvLoop,
    //         file,
    //         _data.getData(),
    //         result -> _callback(resultToOptionCode(result)));
    // }

    // public function read(_callback : Result<Bytes, Code>->Void)
    // {
    //     cpp.luv.File.read(
    //         @:privateAccess Thread.current().events.luvLoop,
    //         file,
    //         data -> _callback(Result.Success(Bytes.ofData(data))),
    //         code -> _callback(Result.Error(new Code(code))));
    // }

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

    /**
     * Generates a function which can be passed into libuv extern functions.
     * The generated function creates a result object depending on if the returned code is an error.
     * If the callback function is null then null is returned.
     * @param _callback Callback function the returned function wraps.
     * @return Wrapper function.
     */
    static function makeWriteCallback(_callback : Null<Result<Int, Code>->Void>) : Null<Code->Void>
    {
        return
            if (_callback == null)
                null
            else
                function (code : Code) {
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