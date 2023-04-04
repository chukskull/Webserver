#include "parsing.hpp"

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
			std::vector<_string> ports;
			_string	host, body_size, name;
			_locations	locations;
			while(std::getline(input_file, line))
			{
				if(line.find("}") != std::string::npos)
					break ;
				if ((j = line.find("listen")) != _string::npos)
				{
					size_t i = escape_white_space(line);
					if (i == j)
						ports.push_back(erase_some_charc(line.substr(line.find("listen") + 7)));
				}
				else if((j = line.find("host")) != _string::npos)
				{
					size_t i = escape_white_space(line);
					// print_error << i << " " << j << std::endl;
					if(i == j)
						host = erase_some_charc(line.substr(line.find("host") + 5));
				}
				else if ((j = line.find("body_size")) != _string::npos)
				{
					size_t	i = escape_white_space(line);
					print_error << i <<'\t' <<j << std::endl;
					if (i == j)
					{
					puts("entering to body_size");
						body_size = erase_some_charc(line.substr(line.find("body_size") + 10));

					}
				}
				else if ((j = line.find("name")) != _string::npos)
				{
					size_t	i = escape_white_space(line);
					if (i == j)
						name = erase_some_charc(line.substr(line.find("name") + 5));
				}
			
			if ((j = line.find("location")) != _string::npos)
			{
				// print_error << "jojojo" << std::endl;
				_string	path, autoindex, root, index;
				size_t	i = escape_white_space(line);
				if(i == j)
				{
					path = erase_some_charc(line.substr(line.find("location") + 9));

				}
				// path = erase_some_charc(path);
				std::vector<_string> 		methods;
				std::pair<bool, _string>	redirec;
				_string	nothing;
				redirec = std::make_pair(false, nothing);
				while (std::getline(input_file, line))
				{

					if (line.find("]") != _string::npos)
						break;
					if ( (j = line.find("autoindex")) != std::string::npos)
					{
						size_t	i = escape_white_space(line);
						if (i == j)
                	    	autoindex = erase_some_charc(line.substr(line.find("autoindex") + 9));
					}
                	else if ((j = line.find("root")) != std::string::npos)
					{
						size_t	i = escape_white_space(line);
						if(i == j)
                	    	root = erase_some_charc(line.substr(line.find("root") + 5));
					}
                	else if ((j = line.find("index")) != std::string::npos)
					{
						size_t	i = escape_white_space(line);
						if (i == j)
                	    	index = erase_some_charc(line.substr(line.find("index") + 6));

					}
                	else if ((j = line.find("method")) != std::string::npos)
					{
						size_t	i = escape_white_space(line);
						if (i == j)
                	    	methods.push_back(erase_some_charc(line.substr(line.find("method") + 7)));
					}
					else if ((j = line.find("redirect")) != _string::npos)
					{
						size_t	i = escape_white_space(line);
						if (i == j)
							redirec = std::make_pair(true, erase_some_charc(line.substr(line.find("redirect") + 9)));
					}
					else
					{
						line = erase_some_charc(line);
						// print_error << line << std::endl;
						if (line.compare("[") != 0)
						{
							print_error << "error configfile" << std::endl;
							return -1;
						}
					}	

				}
				locations.push_back(Location(path, autoindex, index, root, methods, redirec));
				}
			}
			servers.push_back(ServerCongif(ports, body_size, host, name, locations));
		}
	}
	return 0;
}
