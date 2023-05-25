#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "headers.hpp"
class ServerCongif
{
private:
	_string							__body_size;
	std::vector<_string>         	__port;
	_string         				__host;
	std::vector<_string>   			__names;
	_locations						__location;
	std::map<int, _string>			__error;
public:
	ServerCongif(std::vector<_string> port,_string body_size, _string host, std::vector<_string> name, _locations const &locations, std::map<int, _string> _error ): __body_size(body_size),__port(port), __host(host), __names(name), __location(locations), __error(_error)
	{

	}
	std::vector<_string>	get_port()const{
		return __port;
	}
	_string	get_host()const{
		return __host;
	}
	std::vector<_string>	get_name()const{
		return __names;
	}
	_string get_body_size()const
	{
		return __body_size;
	}
	_locations	get_locations()const{
		return __location;
	}
	std::map<int, _string>		get_error()const{
		return	__error;
	}
	~ServerCongif(){};
};

#endif