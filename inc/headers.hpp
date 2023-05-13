#ifndef EVERYTHING_HPP
# define EVERYTHING_HPP



#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <vector>
#include <iterator>
#include <string>
#include <string>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>


class Location;
class ServerCongif;
typedef	std::vector<struct pollfd > _vector_fd;
typedef std::vector<std::string> _methods;
typedef std::vector<Location> _locations;
typedef std::string            _string ;
typedef std::vector<ServerCongif> _server_config;
#define print_error std::cerr
#include "RequestConfig.hpp"

#include "Location.hpp"
#include "ServerCongif.hpp"
#include "client.hpp"

#define	MAX_FD 200
#define PORT 8080
#define BUFFER_SIZE 12000
#define BUFFER_SEND 1200
#include "mesage.hpp"
#include "parsing.hpp"

enum
{
    con_GET = 0,
    con_POST,
    con_DELETE,
};

#endif