package cpp.asio;

import cpp.luv.Luv;

abstract Code(Int) to Int
{
    public function new(_e)
    {
        this = _e;
    }

    public function toString()
    {
        return Luv.errorToString(this);
    }

    public function isError()
    {
        return this < 0;
    }
}