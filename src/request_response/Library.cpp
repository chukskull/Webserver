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
		std::cout << "file::" << it->__file << std::endl;
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
		// std::cout << "auto:" << loc._autoindex << " method:" << method << " GET:" << GET << std::endl;
		if (loc.__path == request_info.requested_file && loc._autoindex == false && request_info.method == GET)
		{
			// this will be a problem in POST requests

			// loc.__file = "index2.html";

			std::cout << "file-:" << loc.__file << "-" << std::endl;
			// std::cout << ":p:" << path << std::endl;
			// std::cout << ":p:" << path.substr(loc.__path.length()) << std::endl;
			// info.file_path = loc.__root + path.substr(loc.__path.length()) + loc.__file;
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

DataConf &servers_library::get_server_index(HTTP_request &request_info, Mesage &msg)
{
	std::vector<DataConf> matched_servers;
	string look_for = _servers[msg._connections.second.first].__host + ":" + _servers[msg._connections.second.first].__port[msg._connections.second.second] ;

	fill_matched_servers(msg, matched_servers);
	return find_best_server_match(request_info, msg, matched_servers);
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

DataConf &servers_library::find_best_server_match(HTTP_request &request_info, Mesage &msg, std::vector<DataConf> &m_servers)
{
	// string look_for = request_info.host.substr(0, request_info.host.find(':'));
	for (std::vector<DataConf>::iterator it = m_servers.begin(); it != m_servers.end(); it++)
	{
		for (std::vector<string>::iterator it_names = it->__names.begin(); it_names != it->__names.end(); it_names++)
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















