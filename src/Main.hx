import cpp.vm.Gc;
import sys.thread.Thread;
import cpp.asio.File;

class Main {
    static function main() {
        Thread.current().events.repeat(() -> {
            trace('gc time');

            Gc.run(true);
        }, 1000);

        File.open('test.txt', OpenMode.Create | OpenMode.Append | OpenMode.WriteOnly, result -> {
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
                    });

                    file.close(() -> {
                        trace('file closed');
                    });
                case Error(code):
                    trace('failed to open file $code');
            }
        });
    }
}