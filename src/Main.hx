import haxe.io.Bytes;
import sys.thread.EventLoop.EventHandler;
import cpp.vm.Gc;
import sys.thread.Thread;
import cpp.asio.File;
import cpp.asio.OpenMode;
import cpp.asio.AccessMode;

class Main {
    static function main() {
        // var handle : EventHandler = null;

        // handle = Thread.current().events.repeat(() -> {
        //     trace('gc time');

        //     Gc.run(true);

        //     Thread.current().events.cancel(handle);
        // }, 1000);

        cpp.asio.TTY.open(Stdout, result -> {
            switch result {
                case Success(tty):
                    tty.write.write(Bytes.ofString('HELLO\r\n'), result -> {
                        switch result {
                            case Some(v):
                                trace(v);
                            case None:
                                trace('done');
                        }

                        tty.close();
                    });
                    // tty.read.read(result -> {
                    //     switch result {
                    //         case Success(data):
                    //             trace(data);
                    //         case Error(error):
                    //             trace(error);
                    //     }

                    //     tty.close();
                    // });
                case Error(error):
                    trace('failed to open stdin ${ error }');
            }
        });

        cpp.asio.Process
            .spawn(
                'C:/Users/AidanLee/Desktop/platformer/.flurry/haxe/Windows/haxe.exe',
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
                                        //
                                }
                            });
                        case Error(error):
                            trace(error);
                    }
                });

        cpp.asio.Directory.open('C:\\Users\\AidanLee\\Desktop\\hxcpp_luv\\src\\cpp\\asio', result -> {
            switch result {
                case Success(dir):
                    dir.iter(result -> {
                        switch result {
                            case Success(entry):
                                trace(entry.name, entry.type);
                            case Error(error):
                                trace(error);

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

        File.open('C:\\Users\\AidanLee\\Desktop\\hxcpp_luv\\test.txt', OpenMode.ReadWrite | OpenMode.Append, AccessMode.UserReadWriteExecute, result -> {
            switch result {
                case Success(file):
                    trace('file opened');

                    file.read(result -> {
                        switch result {
                            case Success(data):
                                trace(data.toString());
                            case Error(error):
                                trace(error);
                        }

                        file.close(result -> {
                            switch result {
                                case Some(v):
                                    trace('failed to close file $v');
                                case None:
                                    trace('file closed');
                            }
                        });
                    });

                case Error(code):
                    trace('failed to open file $code');
            }
        });
    }
}