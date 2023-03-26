#include "everything.hpp"


int parsing_config_file(_string file, int __flag)
{
	std::ifstream	input_file;
	if (__flag == 1)
	{
		input_file(file);
	}
	else
		input_file("server.conf");
	_string         line;
	_server_config  servers;
	while (std::getline(input_file, line))
	{
		if(line.find("server {") != _string::npos)
		{
			_string listen, host, body_size, name;
			_locations	locations;
			while(std::getline(input_file, line))
			{
				if(line.find("}") != std::string::npos)
					break ;
			}
			if (line.find("listen") != _string::npos)
				listen = line.substr(line.find("listen") + 7);
			else if(line.find("host") != _string::npos)
				host = line.substr(line.find("host") + 5);
			else if (line.find("body_size") != _string::npos)
				body_size = line.substr(line.find("body_size") + 10);
			else if (line.find("name") != _string::npos)
				name = line.substr(line.find("name") + 5);
			if (line.find("location") != _string::npos)
			{
				_string	path, autoindex, root, index;
				path = line.substr(line.find("location") + 9);
				std::vector<_string> 		methods;
				std::pair<bool, _string>	redirec;
				while (std::getline(input_file, line))
				{
					if (line.find("]") != _string::npos)
						break;
				
				if (line.find("path") != std::string::npos)
                    path = line.substr(line.find("path") + 5);
				else if (line.find("autoindex") != std::string::npos)
                    autoindex = line.substr(line.find("autoindex") + 9);
                else if (line.find("root") != std::string::npos)
                    root = line.substr(line.find("root") + 5);
                else if (line.find("index") != std::string::npos)
                    index = line.substr(line.find("index") + 6);
                else if (line.find("method") != std::string::npos)
                    methods.push_back(line.substr(line.find("method") + 7));
				if (line.find("redirect") != _string::npos)
					redirec = std::make_pair(true, line.substr(line.find("redirect") + 9));
				else
					redirec = std::make_pair(false, NULL);
				}
				locations.push_back(Location(path, autoindex, index, root, methods, redirec));
				
			}
			servers.push_back(ServerCongif(listen,body_size, host, name,locations));
		}
	}
	
	return 0;
}



int main(int ac, char *av[])
{
	int pars = 0;
	if(ac > 2)
	{
		std::cerr << "too many arguments" << std::endl;
	}
	if (av[1] != NULL)
	{
		_string file(av[1]);
		pars = parsing_config_file(file, 1);
	}
	else
	{
		pars = parsing_config_file(NULL, 0);
	}
	return 0;
}