#include "headers.hpp"
#include <algorithm>


int	phase_to_req_res(_server_config &vec, std::vector<DataConf> &_vec_data)
{
	std::vector<_string>	allow_methods;
	// std::vector<DataConf> _vec_data2;
	allow_methods.push_back("GET");
	allow_methods.push_back("POST");
	allow_methods.push_back("DELETE");
	_server_config::iterator it = vec.begin();
	std::vector<bool>	methods;
	for(; it != vec.end(); it++)
	{
		DataConf _data;
		std::stringstream	lol(it->get_body_size());
		lol >> _data.__body_size;
					// print_error << it->get_body_size() << std::endl;
		if(lol.fail())
			return -1;
		if(it->get_host().size())
			_data.__host = it->get_host();
		if (it->get_name().size())
			_data.__name = it->get_name();
		if(it->get_port().size())
			_data.__port = it->get_port();
		// _data.__body_size = it->get_body_size().a;
		_locations	loc = it->get_locations();
		_locations::iterator	it_2 = loc.begin();
		for (; it_2 != loc.end(); it_2++)
		{
			ReqLoc req_loc;
			if(it_2->get_auto_index().compare("false"))
			{
				req_loc._autoindex = false;
			}
			else if (it_2->get_auto_index().compare("true"))
			{
				req_loc._autoindex = true;
			}
			else
				return -1;
			req_loc.__path = it_2->get_path();
			req_loc.__file = it_2->get_place();
			req_loc.__root = it_2->get_root();
			req_loc.__redirect = it_2->get_redirect();
			_methods	meth = it_2->get_methods();
			_methods::iterator it_3 = meth.begin();
			MethAllow	_allows(3, false);
			print_error << meth.size() << std::endl;
			for(;it_3 != meth.end(); it_3++)
			{
				std::vector<_string>::iterator t_1;
				// print_error << *it_3 << std::endl;
				if ((t_1 = std::find(allow_methods.begin(), allow_methods.end(), *it_3)) == allow_methods.end())
				{
					return -1;
				}
				else{
					size_t index = t_1 - allow_methods.begin();
					_allows[index] = true;
				}
			}
			req_loc._AllowMeth = _allows;
		}
		_vec_data.push_back(_data);
	}
	return 0;
}

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
			servers.push_back(ServerCongif(listen, body_size, host, name, locations));
		}
	}
	return 0;
}



int main(int ac, char *av[])
{
	_server_config			vec;
	std::vector<DataConf> 	_vec_data;
	int pars = 0;
	int pars_2 = 0;
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

		print_error <<"ss " <<it->get_body_size() << std::endl;
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
	pars_2 = phase_to_req_res(vec, _vec_data);
	if(pars_2 < 0)
	{
		exit(0);
	}
	print_error << "thats msn" << std::endl;
	print_error << _vec_data.size() << std::endl;
	// for(size_t i = 0; i < _vec_data.size(); i++)
	// {
		if (_vec_data.size())
			Server::run(_vec_data);
	// }
	return 0;
}