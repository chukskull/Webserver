#include "../../inc/req_res.hpp"

std::string sessionID(int length)
{
    std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::string randomID;

    for (int i = 0; i < length; ++i) {
        int randomIndex = std::rand() % characters.length();
        randomID += characters[randomIndex];
    }
    return randomID;
}

void set_cookies(HTTP_request &request_info, HTTP_response &response)
{
    std::time_t now = std::time(nullptr);
    std::string set_cookies = "Set-Cookie: ";
    std::string sessionid = "SessionID=";
    std::string expires = "expires: ";
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", std::localtime(&now));
    std::string time(buffer);
    if (request_info.cookies.empty())
    {
        std::srand(static_cast<unsigned>(std::time(0)));
        std::string id = sessionID(10);
        response.cookies = set_cookies + sessionid + id + ";" + expires + time;
    }
    else
        response.cookies = set_cookies + request_info.cookies + ";" + expires + time;

}
