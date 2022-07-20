package cpp.asio;

@:structInit
@:publicFields
class ProcessOptions
{
    var args : Array<String> = null;
    var env : Map<String, String> = null;
    var cwd : String = null;
    var shell : Bool = false;
}