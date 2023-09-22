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