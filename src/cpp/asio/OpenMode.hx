package cpp.asio;

@:include('uv.h')
extern class OpenMode
{
    @:native('UV_FS_O_APPEND') public static final Append : Int;
    @:native('UV_FS_O_CREAT') public static final Create : Int;
    @:native('UV_FS_O_DIRECT') public static final Direct : Int;
    @:native('UV_FS_O_DIRECTORY') public static final Directory : Int;
    @:native('UV_FS_O_DSYNC') public static final DSync : Int;
    @:native('UV_FS_O_EXCL') public static final Exclusive : Int;
    @:native('UV_FS_O_EXLOCK') public static final ExclusiveLock : Int;
    @:native('UV_FS_O_FILEMAP') public static final FileMap : Int;
    @:native('UV_FS_O_NOATIME') public static final NoAccessTime : Int;
    @:native('UV_FS_O_NOCTTY') public static final NoControllingTerminal : Int;
    @:native('UV_FS_O_NOFOLLOW') public static final NoFollow : Int;
    @:native('UV_FS_O_NONBLOCK') public static final NonBlocking : Int;
    @:native('UV_FS_O_RANDOM') public static final Random : Int;
    @:native('UV_FS_O_RDONLY') public static final ReadOnly : Int;
    @:native('UV_FS_O_RDWR') public static final ReadWrite : Int;
    @:native('UV_FS_O_SEQUENTIAL') public static final Sequential : Int;
    @:native('UV_FS_O_SYMLINK') public static final SymLink : Int;
    @:native('UV_FS_O_SYNC') public static final Synchronous : Int;
    @:native('UV_FS_O_TEMPORARY') public static final Temporary : Int;
    @:native('UV_FS_O_TRUNC') public static final Truncate : Int;
    @:native('UV_FS_O_WRONLY') public static final WriteOnly : Int;
}