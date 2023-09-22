#include "../../inc/req_res.hpp"
// #include "../../inc/utils.hpp"

request::request()
{
    // start_line.resize(3, string(""));
    start_line.reserve(3);
    body = "";
}

request::request(string req)
{
    start_line.reserve(3);
    body = "";
    parse(req);
    // if (parse(req) == 1)
    //     throw std::exception();
}
void printq(string s)
{
    size_t i = 0;
    while (i < s.size())
    {
        if (s[i] == '\n')
		{
            std::cout << "\\n";
    		std::cout << std::endl;
		}
        else if (s[i] == '\r')
            std::cout << "\\r";
        else
            std::cout << s[i];
        // cout << s[i];
        i++;
    }
}
short request::request_checkpoint()
{
    if (validate_start_line() == 1)
        return 1;
    percent_encoding(start_line[SOURCE]);
    fill_req();
    fill_query(request_info.requested_file, request_info);
    if (request_info.host == "")
    {
        response.set_status(400, "bad request");
        return 1;
    }
    return 0;
}

short request::parse(string req)
{
    std::stringstream s_request(req);
    string tmp_str;
    string line;

    std::getline(s_request, line, '\r');
    std::stringstream start_l_s(line);
    std::getline(s_request, line, '\n');

    short i = 0;
    for (; !start_l_s.eof() ;)
    {
        std::getline(start_l_s, tmp_str, ' ');
        if (tmp_str != "")
        {
            start_line.push_back(tmp_str);
            i++;
        }
    }

    if (i != 3)
    {
        response.set_status(400, "bad request");
        return 1;
    }

    while (std::getline(s_request, tmp_str, '\n') and tmp_str != string("\r"))
    {
        size_t pos = tmp_str.find(":");
        headers.push_back(std::make_pair(string(tmp_str.substr(0, pos)), trim_white_spaces(tmp_str.substr(pos + 1))));
    }

    if (tmp_str != "\r")
    {
        response.set_status(400, "bad request no empty line");
        return 1;
    }
    while (std::getline(s_request, tmp_str))
        {body.append(tmp_str).append("\n");}

    return (request_checkpoint());
}

short request::method_num()
{
    if (start_line[METHOD] == "GET")
        return (GET);
    if (start_line[METHOD] == "POST")
        return (POST);
    if (start_line[METHOD] == "DELETE")
        return (DELETE);
    return -1;
}

int request::fill_req()
{
    request_info.method = method_num();
    request_info.requested_file = start_line[SOURCE];

    for(vec_strp_it it = headers.begin(); it != headers.end(); it++)
    {
        if (_to_lower(it->first) == "host")
        {
            if (request_info.host != "")
            {
                response.set_status(400, "bad request3");
                return 1;
            }
            request_info.host = it->second;
        }
        if (_to_lower(it->first) == "content-type")
        {
            if (request_info.content_type.first != "")
            {
                response.set_status(400, "bad request4");
                return 1;
            }
            fill_content_type(request_info , it->second);
        }
        if (_to_lower(it->first) == "content-length")
        {
            if (request_info.content_length != -1)
            {
                response.set_status(400, "bad request5");
                return 1;
            }
            request_info.content_length = std::stoi(it->second);
            if (request_info.content_length < 0)
            {
                response.set_status(400, "bad request");
                return 1;
            }
        }

        if (_to_lower(it->first) == "connection")
        {
            if (request_info.connection != "")
            {
                response.set_status(400, "bad request6");
                return 1;
            }
            request_info.connection = it->second;
        }
        if (_to_lower(it->first) == "cookie")
        {
            if (request_info.cookies != "")
            {
                response.set_status(400, "bad request3");
                return 1;
            }
            request_info.cookies = it->second;
        }
        if (_to_lower(it->first) == "content-transfer-encoding")
        {
            if (request_info.cookies != "")
            {
                response.set_status(400, "bad request3");
                return 1;
            }
            request_info.content_encoding = it->second;
        }
    }
    request_info.body.swap(body);
    return 0;
}

size_t request::validate_start_line()
{
    if (start_line.size() != 3){response.set_status(400, "bad request line"); return 1;}
    if (start_line[METHOD] == "GET" || start_line[METHOD] == "POST" || start_line[METHOD] == "DELETE")
    {
        if (valid_http(start_line[PROTOCOL]) == false)
        {
            response.set_status(505, "HTTP Version Not Supported");
            return 1;
        }
    }
    else
    {
        response.set_status(501, "Not Implemented");
        return 1;
    }
    return 0;
}

void request::print_req()
{
    for (vec_str_it it = start_line.begin();it != start_line.end(); it++)
    {
        if (it + 1 == start_line.end())
            std::cout << *it << std::endl;
        else
            std::cout << *it << " ";
    }

    std::cout << "host: " << request_info.host << std::endl;
    std::cout << "content_length: " << request_info.content_length << std::endl;
    std::cout << "connection: " << request_info.connection << std::endl;
    std::cout << std::endl;
    std::cout << std::endl << body;
}

void request::print(string s)
{
    size_t i = 0;
    while (i < s.size())
    {
        if (s[i] == '\n')
            std::cout << "\\n";
        else if (s[i] == '\r')
            std::cout << "\\r";
        else
            std::cout << s[i];
        i++;
    }
    std::cout << std::endl;
}

