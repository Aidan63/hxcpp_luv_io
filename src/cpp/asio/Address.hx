package cpp.asio;

class Address
{
    public final address : String;

    public final port : Int;

	public function new(_address, _port)
    {
		address = _address;
		port    = _port;
	}
}