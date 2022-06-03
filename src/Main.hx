import cpp.asio.File;
import cpp.asio.OpenMode;
import cpp.asio.AccessMode;

class Main {
    static function main() {
        // Thread.current().events.repeat(() -> {
        //     trace('gc time');

        //     Gc.run(true);
        // }, 1000);

        cpp.asio.Process
            .spawn(
                'explorer.exe',
                {
                    cwd: '',
                    env: [ "MY_TEST_ENV" => "TRUE" ],
                    args: []
                },
                result -> {
                    switch result {
                        case Success(data):
                            trace(data);
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
                                        case Some(v):
                                            trace(v);
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