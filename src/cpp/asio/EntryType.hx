package cpp.asio;

enum abstract EntryType(Int) to Int
{
    var Unknown;
    var File;
    var Directory;
    var Link;
    var Fifo;
    var Socket;
    var Char;
    var Block;
}