#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "everything.hpp"

class ServerCongif
{
private:
	_string         __port;
	_string         __host;
	_string     	__name;
	_string         __index;
	_locations		__location;
public:
	ServerCongif(_string port, _string host, _string name, _string index, _locations const &locations ):__port(port), __host(host), __name(name), __index(index), __location(locations)
	{

	}
	~ServerCongif();
};




#endif