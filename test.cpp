#include "src/request_response/request.cpp"

int main()
{
    string http_msg = "GET /index.html HTTP/1.1\r\n"
                          "Host: www.example.com\r\n"
                          "content-length: 120\r\n"
                          "Connection: keep-alive\r\n"
                          "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/94.0.4606.61 Safari/537.36\r\n"
                          "\r\n";
    request req(http_msg);
    // req.print_req();
}
