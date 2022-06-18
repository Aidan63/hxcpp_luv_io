Libuv powered asynchronous IO and haxe event loop, more functions and classes will be needed as and when I need them.

### File

```haxe
cpp.asio.File.open(
    'file.txt',
    OpenMode.ReadWrite | OpenMode.Append,
    AccessMode.UserReadWriteExecute,
    result -> {
        switch result {
            case Success(file):
                // 'file' is the object to manipulate the opened file.
                // Below are some examples of file manipulation

                file.read(result -> {
                    switch result {
                        case Success(bytes):
                            // 'bytes' is a `haxe.io.Bytes` object containing the complete file contents.
                        case Error(code):
                            trace('failed to read file data : $code');
                    }
                });

                file.write(Bytes.ofString('hello, world!'), result -> {
                    switch result {
                        case Success(count):
                            // 'count' contains the number of bytes written to the file.
                        case Error(code):
                            trace('failed to write data : $code');
                    }
                });

                file.close(result -> {
                    switch result {
                        case Some(v):
                            trace('failed to close file $v');
                        case None:
                            trace('file closed');
                    }
                });
            case Error(code):
                trace('failed to open file : $code');
        }
    });
```

### Directory

```haxe
cpp.asio.Directory.open(
    'C:/Some/Folder',
    result -> {
        switch result {
            case Success(dir):
                // `dir` is the object to manipulate the opened directory.
                // Currently iterating over the items within the directory is the only supported operation.

                dir.iter(result -> {
                    // This callback can be invoked multiple times, once for each item in the directory.
                    // `Error(Code.eof)` is returned after all items have been iterated.

                    switch result {
                        case Success(entry):
                            trace(entry.name, entry.type);
                        case Error(error):
                            trace(error);

                            // Once error has been raised no further `iter` callbacks will be made

                            dir.close(result -> {
                                switch result {
                                    case Some(error):
                                        trace(error);
                                    case None:
                                        trace('dir closed');
                                }
                            });
                    }
                });
            case Error(error):
                trace(error);
        }
    });
```

### Process

```haxe
cpp.asio.Process.spawn(
    'C:/HaxeToolkit/haxe.exe',
    {
        env: [ "MY_TEST_ENV" => "TRUE", "SOME_DEFINE" => null ],
        args: [ '--help' ]
    },
    result -> {
        switch result {
            case Success(process):
                process.stdout.read(result -> {
                    switch result {
                        case Success(bytes):
                            Sys.print(bytes.toString());
                        case Error(error):
                            trace(error);
                    }
                });
            case Error(error):
                trace(error);
        }
    });
```