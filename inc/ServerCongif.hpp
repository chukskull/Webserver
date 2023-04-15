#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "headers.hpp"
class ServerCongif
{
private:
	_string							__body_size;
	std::vector<_string>         	__port;
	_string         				__host;
	_string     					__name;
	_locations						__location;
public:
	ServerCongif(std::vector<_string> port,_string body_size, _string host, _string name, _locations const &locations ): __body_size(body_size),__port(port), __host(host), __name(name), __location(locations)
	{

	}
	std::vector<_string>	get_port()const{
		return __port;
	}
	_string	get_host()const{
		return __host;
	}
	_string	get_name()const{
		return __name;
	}
	_string get_body_size()const
	{
		return __body_size;
	}
	_locations	get_locations()const{
		return __location;
	}
	~ServerCongif(){};
};

#endif