#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "everything.hpp"

class ServerCongif
{
private:
	int         port;
	int         host;
	_string         name;
	int         index;
	_locations	location;
public:
	ServerCongif(/* args */);
	~ServerCongif();
};

ServerCongif::ServerCongif(/* args */)
{
}

ServerCongif::~ServerCongif()
{
}



#endif