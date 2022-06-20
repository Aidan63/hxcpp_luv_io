package cpp.asio;

extern enum abstract SignalType(Int) to Int
{
    @:native('SIGINT') var Interrupt;
    @:native('SIGBREAK') var Break;
}