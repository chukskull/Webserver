#include "headers.hpp"
#include "Server.hpp"
#include <algorithm>


void	_begin_function(char *av)
{
	std::string	str;
	
	if(av != NULL)
		str = av;
	_server_config			vec;
	std::vector<DataConf>	_vec_data;
	int pars = 0;
	if (str.size() > 0)
	{
		_string file(str);
		pars = parsing_config_file(file, vec);
	}
	else
		pars = parsing_config_file("server.conf", vec);
	if (pars < 0)
		exit(0);
	pars = phase_to_req_res(vec, _vec_data);
	if(pars < 0)
		exit(0);
	if (_vec_data.size())
		Server::run(_vec_data);
}

int main(int ac, char *av[])
{
	if(ac > 2)
		print_error << "too many arguments" << std::endl;
	_begin_function(av[1]);
	return 0;
}
	// print_error << "here" << std::endl;
	// for(_server_config::iterator it = vec.begin(); it != vec.end(); ++it)
	// {
	// 	print_error <<"server - port: " << std::endl;
	// 	std::vector<_string> ports = it->get_port();
	// 	for(size_t	i = 0; i < ports.size(); i++)
	// 	{
	// 		print_error << ports[i] << std::endl;
	// 	}
	// 	print_error << "server - host " << it->get_host() << std::endl;

	// 	print_error <<"ss " <<it->get_body_size() << std::endl;
	// 	print_error << it->get_locations().size() << std::endl;
	// 	for(size_t i = 0; i < it->get_locations().size(); i++)
	// 	{
	// 		print_error << "size of methods : "<< it->get_locations()[i].get_methods().size() << std::endl;
	// 		for(size_t j = 0; j < it->get_locations()[i].get_methods().size(); j++)
	// 		{
	// 			print_error << i << " " << j << std::endl;
	// 			print_error << it->get_locations()[i].get_methods()[j] << std::endl; 
	// 		}
	// 	}
	// }