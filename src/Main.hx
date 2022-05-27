import cpp.vm.Gc;
import cpp.asio.File;
import cpp.asio.OpenMode;
import cpp.asio.AccessMode;
import sys.thread.Thread;
import haxe.Exception;

class Main {
    static function main() {
        Thread.current().events.repeat(() -> {
            trace('gc time');

            Gc.run(true);
        }, 1000);

        File.open('C:\\Users\\AidanLee\\Desktop\\hxcpp_luv\\test.txt', OpenMode.WriteOnly | OpenMode.Create, AccessMode.UserReadWriteExecute, result -> {
            switch result {
                case Success(file):
                    trace('file opened');

                    file.write('hello world!', result -> {
                        switch result {
                            case Some(v):
                                trace('write failed $v');
                            case None:
                                trace('write succeeded');
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