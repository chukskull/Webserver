#include "../../inc/Library.hpp"
#include <queue>

file_info servers_library::get_requested_file(HTTP_request &request_info, DataConf &_server)
{
	file_info info;
	ReqLoc loc;
	size_t pos;

	for (std::vector<ReqLoc>::iterator it = _server.__locations.begin(); it != _server.__locations.end(); it++)
	{
		pos = request_info.requested_file.find(it->__path);
		// std::cout << "file::" << it->__file << std::endl;
		// std::cout << "! get lucky\n";
		if (pos != request_info.requested_file.npos && pos == 0) // possibly check that the npos is zero
		{
			// std::cout << "++++++++++> got here\n";
			if (loc.__path.length() < it->__path.length())
				loc = *it;

		}
	}
	if (loc.is_set())
	{
		info.location = loc;
		info._allowMeth = loc._AllowMeth;
		info.requested_path = request_info.requested_file;
		std::cout << "auto:" << loc._autoindex << " method:" << request_info.method << " GET:" << GET << std::endl;
		if (loc.__path == request_info.requested_file && loc._autoindex == false && request_info.method == GET)
		{
			// this will be a problem in POST requests

			// loc.__file = "index2.html";

			std::cout << "file-:" << loc.__file << "-" << std::endl;
			std::cout << ":p:" << request_info.requested_file << std::endl;
			std::cout << ":p:" << request_info.requested_file.substr(loc.__path.length()) << std::endl;
			info.file_path = loc.__root + request_info.requested_file.substr(loc.__path.length()) + loc.__file;
		}
		else
		{
			info.file_path = loc.__root + request_info.requested_file.substr(loc.__path.length());
		}

		info.content_type = get_exten(info.file_path);
		// std::cout << "the glory type: " << info.content_type << std::endl;
		// info.type = path.substr(path.rfind('.') + 1);

		if (loc.__redirect.first)
		{
			info.is_redirect = true;
		}
		else
		{
			// check if the file exists
			if (file_exist(info.file_path))
			{
				info.file_exists = true;
				info._allowMeth = loc._AllowMeth;
				// check if it is a directory
				if (is_dir(info.file_path))
				{
					info.is_dir = true;
					// check if autoindex is allowed
					// std::cout << "the file is a directory\n";

					if (loc._autoindex)
					{
						std::cout << "autoindex is allowed\n";
						info.is_autoindex = true;
					}
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
	// string look_for = request_info.host.substr(0, request_info.host.find(':'));
	for (std::vector<DataConf>::iterator it = m_servers.begin(); it != m_servers.end(); it++)
	{
		for (std::vector<string>::iterator it_names = it->__name.begin(); it_names != it->__name.end(); it_names++)
		{
			for(std::vector<string>::iterator it_port = it->__port.begin(); it_port != it->__port.end(); it_port++)
			{
				// std::cout << "===>" << request_info.host;
				// std::cout << "===>" << *it_names << std::endl;
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

void servers_library::create_status_map(map<int, string> &status_map) {

    status_map[200] = "OK";
    status_map[201] = "Created";
    status_map[202] = "Accepted";
    status_map[301] = "Moved Permanently";
    status_map[400] = "Bad Request";
    status_map[403] = "Forbidden";
    status_map[404] = "Not Found";
    status_map[405] = "Method Not Allowed";
    status_map[408] = "Request Timeout";
    status_map[411] = "Length Required";
    status_map[412] = "Precondition Failed";
    status_map[413] = "Request Entity Too Large";
    status_map[415] = "Unsupported Media Type";
    status_map[500] = "Internal Server Error";
    status_map[501] = "Not Implemented";
    status_map[502] = "Bad Gateway";
    status_map[504] = "Gateway Timeout";
    status_map[505] = "HTTP Version Not Supported";
}

void servers_library::set_error_pages(const std::map<short, std::string>& error_pages) {
	html_error_pages.clear(); // Clear existing error pages
	
	// Iterate over the error_pages map
	for (std::map<short, std::string>::const_iterator it = error_pages.begin(); it != error_pages.end(); ++it) {
		short status_code = it->first;
		const std::string& file_name = it->second;

		std::ifstream file(file_name.c_str());
		if (file.is_open()) {
			std::stringstream buffer;
			buffer << file.rdbuf();
			std::string file_content = buffer.str();

			html_error_pages[status_code] = file_content;

			file.close();
		} else {
			if (status.find(status_code) != status.end())
				html_error_pages[status_code] = generate_error_page(status_code, status.find(status_code)->second);
			else
				html_error_pages[status_code] = generate_error_page(status_code, "");
		}
	}
}

string servers_library::get_error_page(short status_code)
{
	map<short, string>::iterator it;

	it = html_error_pages.find(status_code);
	if (it != html_error_pages.end())
		return it->second;
	else
		return generate_error_page(status_code, status.find(status_code)->second);
}











