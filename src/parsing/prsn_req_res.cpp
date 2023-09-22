#include "parsing.hpp"

int	phase_to_req_res(_server_config &vec, std::vector<DataConf> &_vec_data)
{
	std::vector<_string>	allow_methods;
	allow_methods.push_back("GET");
	allow_methods.push_back("POST");
	allow_methods.push_back("DELETE");
	_server_config::iterator it = vec.begin();
	std::vector<bool>	methods;
	for(;it != vec.end(); it++)
	{
		DataConf _data;
		std::stringstream	lol(it->get_body_size());
		lol >> _data.__body_size;
		if(lol.fail())
			return -1;
		if(it->get_host().size())
			_data.__host = it->get_host();
		if (it->get_name().size())
			_data.__name = it->get_name();
		if(it->get_port().size())
			_data.__port = it->get_port();
		if (it->get_error().size())
			_data.__error = it->get_error();
		_locations	loc = it->get_locations();
		_locations::iterator	it_2 = loc.begin();
		for (; it_2 != loc.end(); it_2++)
		{
			ReqLoc req_loc;

			if(it_2->get_auto_index().compare("false") == 0)
				req_loc._autoindex = false;
			else if (it_2->get_auto_index().compare("true") == 0)
				req_loc._autoindex = true;
			else
				return -1;
			req_loc.__path = it_2->get_path();
			if (req_loc.__path.back() != '/')
				req_loc.__path += '/';
			req_loc.__file = it_2->get_place();
			req_loc.__root = it_2->get_root();
			req_loc.__redirect = it_2->get_redirect();
			req_loc._cgi = it_2->get_cgi_bool();
			req_loc.__cgi_path = it_2->get_cgi_path();
			req_loc.__cgi_ext = it_2->get_cgi_ext();
			_methods	meth = it_2->get_methods();
			_methods::iterator it_3 = meth.begin();
			MethAllow	_allows(3, false);
			for(;it_3 != meth.end(); it_3++)
			{
				std::vector<_string>::iterator t_1;
				if ((t_1 = std::find(allow_methods.begin(), allow_methods.end(), *it_3)) == allow_methods.end())
					return -1;
				else
				{
					size_t index = t_1 - allow_methods.begin();
					_allows[index] = true;
				}
			}
			req_loc._AllowMeth = _allows;
			_data.__locations.push_back(req_loc);
		}
		_vec_data.push_back(_data);
	}
	return 0;
}
