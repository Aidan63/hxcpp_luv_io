package cpp.asio;

@:include('uv.h')
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

    @:op(a | b) static function or(a : OpenMode, b : OpenMode) : OpenMode;
    @:op(a & b) static function or(a : OpenMode, b : OpenMode) : OpenMode;
}