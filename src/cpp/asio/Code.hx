package cpp.asio;

import cpp.luv.Luv;

abstract Code(Int) to Int
{
    public static inline final eof : Code = cast -4095;

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