#include "parsing.hpp"



bool	catch_elem(_string _confg, size_t j)
{
	size_t i = escape_white_space(_confg);
					
	if(i == j)
		return true;
	return false;
}

int parsing_config_file(_string file, _server_config &servers)
{
	std::ifstream	input_file;
	_string         line;
	
	input_file.open(file);
	if(!input_file.is_open())
		return -1;
	while (std::getline(input_file, line))
	{
		size_t j = 0;
		if(line.find("server {") != _string::npos)
		{
			std::vector<_string> 		ports;
			std::vector<_string> 		names;
			_string						host, body_size;
			std::map<int, _string>		error;
			_locations	locations;
			while(std::getline(input_file, line))
			{
				bool			check = false;
				if(line.find("}") != std::string::npos)
					break ;
				if ((j = line.find("listen")) != _string::npos && catch_elem(line, j))
				{
					check = true;
					ports.push_back(erase_some_charc(line.substr(line.find("listen") + 7)));
				}

				else if((j = line.find("host")) != _string::npos && catch_elem(line, j))
				{
					check = true;
					host = erase_some_charc(line.substr(line.find("host") + 5));
				}

				else if ((j = line.find("body_size")) != _string::npos && catch_elem(line, j))
				{
					check = true;
					body_size = erase_some_charc(line.substr(line.find("body_size") + 10));
				}

				else if ((j = line.find("name")) != _string::npos && catch_elem(line, j))
				{
					check = true;
					names.push_back(erase_some_charc(line.substr(line.find("name") + 5)));
				}
				else if ((j = line.find("error")) != _string::npos && catch_elem(line, j))
				{
					check = true;
					_string		status;
					_string		str;
					int			status_2;
					bool		for_str = false;
					bool 		is_it_last = false;
					for(size_t i = j + 5; i < line.size(); i++)
					{
						if(isspace(line[i]))
						{
							if (is_it_last)
								return - 1;
							;
						}
						else if (for_str)
						{
							str += line[i];
							is_it_last = true;
						}
						else
						{
							status += line[i];
							if (isspace(line[i + 1]))
							{
								for_str = true;
								if (is_digit(status))
									status_2 = atoi(status.c_str());
								else
									return -1;
							}
						}
					}
					error.insert(std::make_pair(status_2,erase_some_charc(str)));
				}
				//location part			
				if ((j = line.find("location")) != _string::npos && catch_elem(line, j))
				{
						_string	path, autoindex, root, index ,cgi_path, cgi_extension;
						path = erase_some_charc(line.substr(line.find("location") + 9));
						std::vector<_string> 		methods;
						std::pair<bool, _string>	redirec;
						_string	nothing;
						redirec = std::make_pair(false, nothing);
					while (std::getline(input_file, line))
					{
						if (line.find("]") != _string::npos)
							break;
						if ( (j = line.find("autoindex")) != std::string::npos && catch_elem(line, j))
						{
							check = true;
                		    autoindex = erase_some_charc(line.substr(line.find("autoindex") + 9));
						}

                		else if ((j = line.find("root")) != std::string::npos && catch_elem(line, j))
                		    root = erase_some_charc(line.substr(line.find("root") + 5));

                		else if ((j = line.find("index")) != std::string::npos && catch_elem(line, j))
                		    index = erase_some_charc(line.substr(line.find("index") + 6));

                		else if ((j = line.find("method")) != std::string::npos && catch_elem(line, j))

                		    methods.push_back(erase_some_charc(line.substr(line.find("method") + 7)));
						else if ((j = line.find("redirect")) != _string::npos && catch_elem(line, j))
							redirec = std::make_pair(true, erase_some_charc(line.substr(line.find("redirect") + 9)));

						else if ((j = line.find("cgi_path")) != _string::npos && catch_elem(line, j))
							cgi_path = erase_some_charc(line.substr(line.find("cgi_path") + 9));
						else if ((j = line.find("cgi_extension")) != _string::npos && catch_elem(line, j))
							cgi_extension = erase_some_charc(line.substr(line.find("cgi_extension") + 14));
						else
						{
							line = erase_some_charc(line);
							if (line.compare("[") != 0 && line.size() != 0)
							{
								print_error << "error configfile" << std::endl;
								return -1;
							}
						}
					}
					if(cgi_extension.size() && cgi_path.size())
						locations.push_back(Location(path, autoindex, index, root, methods,true, cgi_path, cgi_extension, redirec));
					else if (cgi_extension.size() || cgi_path.size())
					{
						print_error << "error config file" << std::endl;
						return -1;
					}
					else
					{
						locations.push_back(Location(path, autoindex, index, root, methods, false,cgi_path, cgi_extension, redirec));
					}
				}
				if (check == false)
				{
					line = erase_some_charc(line);
					if (line.size() > 0)
					{
						print_error << "error config file" << std::endl;
						return -1;
					}
				}
			}
			servers.push_back(ServerCongif(ports, body_size, host, names, locations, error));
		}
		else
		{
			line = erase_some_charc(line);
			if (line.size() > 0)
			{
				print_error << "error config file" << std::endl;
				return -1;
			}
		}
	}
	return 0;
}
