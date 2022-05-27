package cpp.asio;

enum abstract AccessMode(Int) to Int
{
    var UserReadWriteExecute = 700;
    var UserRead = 400;
    var UserWrite = 200;
    var UserExecute = 100;

    var GroupReadWriteExecute = 70;
    var GroupRead = 40;
    var GroupWrite = 20;
    var GroupExecute = 10;

    var OthersReadWriteExecute = 7;
    var OthersRead = 4;
    var OthersWrite = 2;
    var OthersExecute = 1;

    @:op(a | b) static function or(a : AccessMode, b : AccessMode) : AccessMode;
    @:op(a & b) static function or(a : AccessMode, b : AccessMode) : AccessMode;
}