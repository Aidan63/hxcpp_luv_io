package cpp.asio;

enum Result<T, E>
{
    Success(data : T);
    Error(error : E);
}