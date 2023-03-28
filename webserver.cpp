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
						listen = erase_some_charc(line.substr(line.find("listen") + 7));
				}
				else if((j = line.find("host")) != _string::npos)
				{
					size_t i = escape_white_space(line);
					// print_error << i << " " << j << std::endl;
					if(i == j)
						host = erase_some_charc(line.substr(line.find("host") + 5));
				}
				else if (line.find("body_size") != _string::npos)
				{
					size_t	i = escape_white_space(line);
					if (i == j)
						body_size = erase_some_charc(line.substr(line.find("body_size") + 10));
				}
				else if (line.find("name") != _string::npos)
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
					path = erase_some_charc(line.substr(line.find("location") + 9));
				// path = erase_some_charc(path);
				std::vector<_string> 		methods;
				std::pair<bool, _string>	redirec;
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
					if ((j = line.find("redirect")) != _string::npos)
					{
						size_t	i = escape_white_space(line);
						if (i == j)
							redirec = std::make_pair(true, erase_some_charc(line.substr(line.find("redirect") + 9)));
					}
					else
					{
						_string	nothing;
						redirec = std::make_pair(false, nothing);
					}
				}
				locations.push_back(Location(path, autoindex, index, root, methods, redirec));
				}
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
		print_error << it->get_locations().size() << std::endl;
		for(size_t i = 0; i < it->get_locations().size(); i++)
		{
			print_error << "size of methods : "<< it->get_locations()[i].get_methods().size() << std::endl;
			for(size_t j = 0; j < it->get_locations()[i].get_methods().size(); j++)
			{
				print_error << i << " " << j << std::endl;
				print_error << it->get_locations()[i].get_methods()[j] << std::endl; 
			}
		}
	}
	return 0;
}