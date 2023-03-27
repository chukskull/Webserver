#include "headers.hpp"

bool isWhitespace(char c) 
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}
size_t	escape_white_space(_string my_string)
{
	size_t k = 0;
	for(; k < my_string.size(); k++)
	{
		if(!isWhitespace(my_string[k]))
			break ;
	}
	return k;
}


_string		erase_some_charc(_string my_string)
{
	std::vector<char> sure;
	sure.push_back(' ');
	sure.push_back('\t');
	sure.push_back('\n');
	sure.push_back('\r');
	sure.push_back(';');
	for (size_t i = 0; i < sure.size(); i++)
	{
		my_string.erase(remove(my_string.begin(), my_string.end(), sure[i]), my_string.end());
	}
	return my_string;
}

int parsing_config_file(_string file, _server_config &servers)
{
	std::ifstream	input_file;
	_string         line;
	
	input_file.open(file);
	if(!input_file.is_open())
		return -1;
	// _server_config  servers;
	while (std::getline(input_file, line))
	{
		size_t j = 0;
		if(line.find("server {") != _string::npos)
		{
			_string listen, host, body_size, name;
			_locations	locations;
			while(std::getline(input_file, line))
			{
				if(line.find("}") != std::string::npos)
					break ;
				if ((j = line.find("listen")) != _string::npos)
				{
					size_t i = escape_white_space(line);
					if (i == j)
					{
						listen = line.substr(line.find("listen") + 7);
						listen = erase_some_charc(listen);
					}
					// listen.erase(remove(listen.begin(), listen.end(), 'p'), listen.end());
				}
				else if((j = line.find("host")) != _string::npos)
				{
					print_error << line.size() << std::endl;
					size_t i = escape_white_space(line);
					print_error << i << " " << j << std::endl;
					if(i == j)
					{
						host = line.substr(line.find("host") + 5);
					}
					// print_error << host << std::endl;
					// host = erase_some_charc(host);
				}
				else if (line.find("body_size") != _string::npos)
				{
					body_size = line.substr(line.find("body_size") + 10);
					body_size = erase_some_charc(body_size);
				}
				else if (line.find("name") != _string::npos)
				{
					name = line.substr(line.find("name") + 5);
					name = erase_some_charc(name);
				}
			}
			if (line.find("location") != _string::npos)
			{
				_string	path, autoindex, root, index;
				path = line.substr(line.find("location") + 9);
				path = erase_some_charc(path);
				std::vector<_string> 		methods;
				std::pair<bool, _string>	redirec;
				while (std::getline(input_file, line))
				{
					if (line.find("]") != _string::npos)
						break;
				
				if (line.find("path") != std::string::npos)
                    path = line.substr(line.find("path") + 5);
				else if (line.find("autoindex") != std::string::npos)
				{
                    autoindex = line.substr(line.find("autoindex") + 9);
					autoindex = erase_some_charc(autoindex);

				}
                else if (line.find("root") != std::string::npos)
				{
                    root = line.substr(line.find("root") + 5);
					root = erase_some_charc(root);
				}
                else if (line.find("index") != std::string::npos)
				{
                    index = line.substr(line.find("index") + 6);
					root = erase_some_charc(root);

				}
                else if (line.find("method") != std::string::npos)
				{
                    methods.push_back(erase_some_charc(line.substr(line.find("method") + 7)));

				}
				if (line.find("redirect") != _string::npos)
					redirec = std::make_pair(true, erase_some_charc(line.substr(line.find("redirect") + 9)));
				else
				{
					_string	nothing;
					redirec = std::make_pair(false, nothing);
				}
				}
				locations.push_back(Location(path, autoindex, index, root, methods, redirec));
				
			}
			servers.push_back(ServerCongif(listen, body_size, host, name, locations));
		}
	}
	return 0;
}



int main(int ac, char *av[])
{
	_server_config	vec;
	int pars = 0;
	if(ac > 2)
	{
		print_error << "too many arguments" << std::endl;
	}
	if (av[1] != NULL)
	{
		_string file(av[1]);
		pars = parsing_config_file(file, vec);
	}
	else
		pars = parsing_config_file("server.conf", vec);
	if (pars < 0)
		exit(0);
	print_error << "here" << std::endl;
	for(_server_config::iterator it = vec.begin(); it != vec.end(); ++it)
	{
		print_error <<"server - port: " << it->get_port()  << std::endl;
		print_error << "server - host " << it->get_host() << std::endl;
	}
	return 0;
}