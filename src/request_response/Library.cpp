#include "../../inc/Library.hpp"
#include <queue>

file_info servers_library::get_requested_file(HTTP_request &request_info, DataConf &_server)
{
	file_info info;
	ReqLoc loc;
	size_t pos;

	for (std::vector<ReqLoc>::iterator it = _server.__locations.begin(); it != _server.__locations.end(); it++)
	{
		if (it->__path[it->__path.length() - 1] == '/')
			it->__path = it->__path.substr(0, it->__path.length() - 1);
		pos = request_info.requested_file.find(it->__path);
		if (pos != request_info.requested_file.npos && pos == 0) // possibly check that the npos is zero
		{
			if (loc.__path.length() <= it->__path.length())
			{
				if ((it->__path.length() == request_info.requested_file.length()) || request_info.requested_file[it->__path.length()] == '/')
					loc = *it;
			}
		}
	}
	if (loc.is_set())
	{
		if (loc.__path[loc.__path.length() - 1] == '/')
			loc.__path = loc.__path.substr(0, loc.__path.length() - 1);
		if (request_info.requested_file[request_info.requested_file.length() - 1] == '/')
			request_info.requested_file = request_info.requested_file.substr(0, request_info.requested_file.length() - 1);
		info.location = loc;
		info._allowMeth = loc._AllowMeth;
		info.requested_path = request_info.requested_file;
		if (loc.__path == request_info.requested_file && loc._autoindex == false && request_info.method == GET)
			info.file_path = loc.__root + request_info.requested_file.substr(loc.__path.length()) + loc.__file;
		else
			info.file_path = loc.__root + request_info.requested_file.substr(loc.__path.length());

		info.content_type = get_exten(info.file_path);

		if (loc.__redirect.first)
			info.is_redirect = true;
		else
		{
			string dir = get_dir(info.file_path);
			if (file_exist(dir))
			{
				info.file_dir_exists = true;
				if (file_is_readable(dir))
					info.file_dir_readable = true;
				if (file_is_writable(dir))
					info.file_dir_writable = true;
			}
			if (file_exist(info.file_path))
			{
				if (file_is_readable(info.file_path))
					info.is_readable = true;
				if (file_is_writable(info.file_path))
					info.is_writable = true;
				info.file_exists = true;
				info._allowMeth = loc._AllowMeth;
				if (is_dir(info.file_path))
				{
					info.is_dir = true;
					if (loc._autoindex)
						info.is_autoindex = true;
				}
				else if (is_file(info.file_path))
				{
					info.is_file = true;
				}
			}
		}
	}
	return info;
}

void servers_library::set(vector<DataConf> &srvrs)
{
	_servers = srvrs;
	create_status_map();
}

DataConf servers_library::get_server_index(HTTP_request &request_info, Mesage &msg)
{
	std::vector<DataConf> matched_servers;
	string look_for = _servers[msg._connections.second.first].__host + ":" + _servers[msg._connections.second.first].__port[msg._connections.second.second] ;
	fill_matched_servers(msg, matched_servers);
	if (matched_servers.size() > 1)
		return find_best_server_match(request_info, matched_servers);
	return *(matched_servers.begin());
}

void servers_library::fill_matched_servers( Mesage &msg, std::vector<DataConf> &m_servers)
{
	for (std::vector<DataConf>::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		if (matched_host(*it, msg._connections.second.first) && matched_port(*it, msg))
		{
			m_servers.push_back(*it);
		}
	}
}

DataConf &servers_library::find_best_server_match(HTTP_request &request_info, std::vector<DataConf> &m_servers)
{
	for (std::vector<DataConf>::iterator it = m_servers.begin(); it != m_servers.end(); it++)
	{
		for (std::vector<string>::iterator it_names = it->__name.begin(); it_names != it->__name.end(); it_names++)
		{
			for(std::vector<string>::iterator it_port = it->__port.begin(); it_port != it->__port.end(); it_port++)
			{
				if (*it_names + ":" + *it_port == request_info.host)
				{
					return *it;
				}
			}
		}
	}
	return *(m_servers.begin());
}

bool servers_library::matched_host(DataConf &to_check, size_t index)
{
	if (to_check.__host == _servers[index].__host)
		return true;
	return false;
}

bool servers_library::matched_port(DataConf &to_check, Mesage &msg)
{
	for (std::vector<string>::iterator it = to_check.__port.begin(); it != to_check.__port.end(); it++)
	{
		if (*it == _servers[msg._connections.second.first].__port[msg._connections.second.second])
			return true;
	}
	return false;
}


string servers_library::generate_error_page(int status_code, string status_message)
{
    std::ostringstream oss;
    oss << "<!DOCTYPE html>\n"
        << "<html>\n"
        << "<head>\n"
        << "    <title>Error " << status_code << "</title>\n"
        << "    <style>\n"
        << "        body {\n"
        << "            font-family: Arial, sans-serif;\n"
        << "            background-color: #f4f4f4;\n"
        << "            margin: 0;\n"
        << "            padding: 0;\n"
        << "        }\n"
        << "        .container {\n"
        << "            max-width: 600px;\n"
        << "            margin: 50px auto;\n"
        << "            padding: 20px;\n"
        << "            background-color: #fff;\n"
        << "            border: 1px solid #ccc;\n"
        << "            border-radius: 5px;\n"
        << "            box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);\n"
        << "        }\n"
        << "        h1 {\n"
        << "            color: #333;\n"
        << "        }\n"
        << "        p {\n"
        << "            color: #777;\n"
        << "        }\n"
        << "    </style>\n"
        << "</head>\n"
        << "<body>\n"
        << "    <div class=\"container\">\n"
        << "        <h1>Error " << status_code << "</h1>\n"
        << "        <p>" << status_message << "</p>\n"
        << "    </div>\n"
        << "</body>\n"
        << "</html>";

    return oss.str();
}

void servers_library::create_status_map() {

    status[200] = "OK";
    status[201] = "Created";
    status[202] = "Accepted";
    status[301] = "Moved Permanently";
    status[400] = "Bad Request";
    status[403] = "Forbidden";
    status[404] = "Not Found";
    status[405] = "Method Not Allowed";
    status[408] = "Request Timeout";
    status[411] = "Length Required";
    status[412] = "Precondition Failed";
    status[413] = "Request Entity Too Large";
    status[415] = "Unsupported Media Type";
    status[500] = "Internal Server Error";
    status[501] = "Not Implemented";
    status[502] = "Bad Gateway";
    status[504] = "Gateway Timeout";
    status[505] = "HTTP Version Not Supported";
}

void servers_library::set_error_pages(const std::map<int, std::string>& error_pages) {
	html_error_pages.clear();
	
	for (std::map<int, std::string>::const_iterator it = error_pages.begin(); it != error_pages.end(); ++it)
	{
		short status_code = it->first;
		const std::string& file_name = it->second;

		std::ifstream file(file_name.c_str());
		if (file.is_open()) {
			std::stringstream buffer;
			buffer << file.rdbuf();
			std::string file_content = buffer.str();

			html_error_pages[status_code] = file_content;

			file.close();
		}
		else
		{
			if (status.find(status_code) != status.end())
				html_error_pages[status_code] = generate_error_page(status_code, status.find(status_code)->second);
			else
				html_error_pages[status_code] = generate_error_page(status_code, "");
		}
	}
}

string servers_library::get_error_page(short status_code, string status_text)
{
	map<short, string>::iterator it;

	it = html_error_pages.find(status_code);
	if (it != html_error_pages.end())
	{
		return it->second;
	}
	else
	{
		if (status_text == "")
			return generate_error_page(status_code, status.find(status_code)->second);
		else
			return generate_error_page(status_code, status_text);
	}
}

string servers_library::get_status_text(short status_code)
{
	map<short, string>::iterator it;

	it = status.find(status_code);
	if (it != status.end())
		return it->second;
	else
		return "not suported status";
}

servers_library::servers_library(vector<DataConf> servers) : _servers(servers){create_status_map();}

std::string servers_library::generate_success_page(short status_code, const std::string& status_text)
{
    std::stringstream ss;
    ss << "<!DOCTYPE html>\n";
    ss << "<html>\n";
    ss << "<head>\n";
    ss << "    <title>Success " << status_code << "</title>\n";
    ss << "    <style>\n";
    ss << "        body {\n";
    ss << "            background-color: #333333;\n";
    ss << "            font-family: Arial, sans-serif;\n";
    ss << "            padding: 20px;\n";
    ss << "            text-align: center;\n";
    ss << "        }\n";
    ss << "        .card {\n";
    ss << "            background-color: #222222;\n";
    ss << "            border-radius: 8px;\n";
    ss << "            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);\n";
    ss << "            color: #ffffff;\n";
    ss << "            margin: 0 auto;\n";
    ss << "            max-width: 500px;\n";
    ss << "            padding: 40px;\n";
    ss << "        }\n";
    ss << "        h1 {\n";
    ss << "            color: #00a95c;\n";
    ss << "            font-size: 24px;\n";
    ss << "            margin-bottom: 20px;\n";
    ss << "        }\n";
    ss << "        p {\n";
    ss << "            font-size: 16px;\n";
    ss << "            line-height: 1.5;\n";
    ss << "            margin-bottom: 20px;\n";
    ss << "        }\n";
    ss << "    </style>\n";
    ss << "</head>\n";
    ss << "<body>\n";
    ss << "    <div class=\"card\">\n";
    ss << "        <h1>Success " << status_code << "</h1>\n";
    ss << "        <p>" << status_text << "</p>\n";
    ss << "    </div>\n";
    ss << "</body>\n";
    ss << "</html>\n";

    return ss.str();
}


string servers_library::get_success_page(short status_code)
{
	return generate_success_page(status_code, status.find(status_code)->second);
}





