#ifndef SERVER_HPP
# define SERVER_HPP
#include "headers.hpp"


class Server {
public:
    Server(std::vector<DataConf> &data):_containers(data) {
    }

    static std::pair<size_t, size_t>   find_the_pair_connection(std::vector<std::pair<size_t, size_t> > &_concec, int fd)
    {
        
        for(std::vector<std::pair<size_t, size_t> >::iterator it = _concec.begin(); it != _concec.end(); it++)
        {
            if (it->first == static_cast<size_t>(fd))
            {
                return *it;
            }
        }
       return *_concec.end();
    }
	void	initial_server(std::vector<DataConf> &data)
	{
        bzero(&hints, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;
        for(size_t  i = 0; i < data.size(); i++)
        {
            for(size_t j = 0; j < data[i].__port.size(); j++)
            {
                int server_fd;
                print_error << data[i].__port[j].c_str() << std::endl;
                if ((rcv = getaddrinfo(data[i].__host.c_str() , data[i].__port[j].c_str(), &hints, &ai)) != 0) {
                    throw std::string("error in getaddrinfo");
                }
                print_error << "here" << std::endl;
                print_error << " cringe " << std::endl;
                for (point = ai; point != NULL; point = point->ai_next) {
                    server_fd = socket(point->ai_family, point->ai_socktype, point->ai_protocol);
                    if (server_fd < 0)
                        continue;
                    else
                        this->server_fds.push_back(server_fd);
                    this->on = 1;
                    rc = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
                    if (rc < 0) {
                        throw std::string("error with setsocketopt");
                    }
                    if (fcntl(server_fd, F_SETFL, O_NONBLOCK) < 0)
                    {
                        throw std::string("error fcntl");
                    }
                    if (bind(server_fd, point->ai_addr, point->ai_addrlen) < 0) {
                        close(server_fd);
                        continue;
                    }
                    break ;
                }
                freeaddrinfo(ai);
                if (point == NULL) {
		            	throw std::string("there's no ip available in this host");
                }
                if (listen(server_fd, 1) == -1)
                    throw std::string("problem with listen");
                else
                    print_error << server_fd << std::endl;
                pollfd  temp;
                temp.fd = server_fd;
                temp.events = POLLIN;
                fd_s.push_back(temp);
                timeout = (2 * 60 * 1000);
                fd_counts = 1;
            }
        }
	}
    ~Server() {}

   static void run(std::vector<DataConf> &__vec_data) {
    print_error << "rijal" << std::endl;
        Server  __my_ser(__vec_data);
		try
		{
			__my_ser.initial_server(__vec_data);
		}
		catch(const std::string e)
		{
			std::cerr << e << '\n';
		}
		
        while (1) {
            __my_ser.rc = 1;
            __my_ser.rc = poll(&__my_ser.fd_s[0], __my_ser.fd_s.size(), __my_ser.timeout);
            if (__my_ser.rc < 0) {
                perror("poll() failed");
                break ;
            }
            if (__my_ser.rc == 0) {
                std::cerr << "poll() timed out , End program\n" << std::endl;
                break ;
            }
            for (size_t i = 0; i < __my_ser.fd_s.size(); i++) {
                if (__my_ser.fd_s[i].revents & POLLIN) {
                    print_error << __my_ser.fd_s[i].fd << "rumble " << std::endl;
                    size_t  index;
                    std::vector<int>::iterator find_;
                    if ((find_ = find(__my_ser.server_fds.begin(), __my_ser.server_fds.end(), __my_ser.fd_s[i].fd)) != __my_ser.server_fds.end()) {
                        __my_ser.addrlen = sizeof __my_ser.remoteaddr;
                        index = __my_ser.server_fds[find_ - __my_ser.server_fds.begin()];
                        int newfd = accept(__my_ser.server_fds[find_ - __my_ser.server_fds.begin()], (struct sockaddr *) &__my_ser.remoteaddr, &__my_ser.addrlen);
                        // print_error << __my_ser.server_fds[find_ - __my_ser.server_fds.begin()] << " " << std::endl;

                        if (newfd == -1)
                            perror("accept");
                        else {
                            __my_ser.fd_s.push_back(pollfd());
                            __my_ser.fd_s.back().fd = newfd;
                            __my_ser.fd_s.back().events = POLLIN;
                            __my_ser.fd_counts++;
                            __my_ser._connections.push_back(std::make_pair(newfd, index));
                        }
                    } else {
                        int bytes = recv(__my_ser.fd_s[i].fd, __my_ser.buf, BUFFER_SIZE, 0);
                        __my_ser.buf[bytes] = '\0';
                        std::pair<size_t, size_t>   pair_found = find_the_pair_connection(__my_ser._connections, __my_ser.fd_s[i].fd);
                        Mesage  *mesg = new Mesage();
                        std::string message(__my_ser.buf);
                        mesg->message = message;
                        mesg->_connections = pair_found;
                        int sender_fd = __my_ser.fd_s[i].fd;
                        print_error << message << std::endl;
                        print_error << "object send to ayman " <<mesg->_connections.first << " " <<mesg->_connections.second << std::endl;
                        if (bytes <= 0) {
                            if (bytes == 0) 
							{
								std::cerr << "this client hung up " << sender_fd<< std::endl;
							}
							else
							{
                                // write(0, __my_ser.buf, sizeof __my_ser.buf);
								perror("rev");
							}
						
						close(__my_ser.fd_s[i].fd);
						__my_ser.fd_s[i] = __my_ser.fd_s.back();
                        std::vector<std::pair<size_t, size_t> >::iterator it_pair_found = std::find(__my_ser._connections.begin(), __my_ser._connections.end(), pair_found);
                        __my_ser._connections.erase(it_pair_found);
						__my_ser.fd_s.pop_back();
					}
					else
						{
							// this part gonna send back the response to a client
							// write(terminal_fd, message.c_str(), message.length());	
						}
					}
				}
			}
		}
	}
		std::vector<int>					server_fds;
		int									rcv;
		int									on;
		int							        rc;
		int							        nfds;
		int									timeout;
		int									fd_counts;
		int									current_size;
		struct	addrinfo					    hints, *ai, *point;
		struct	sockaddr_storage			        remoteaddr;
		socklen_t 							        addrlen;
		char								        buf[BUFFER_SIZE + 1];
		_vector_fd	                                fd_s;
        std::vector<DataConf>                       _containers;
        std::vector<std::pair<size_t, size_t> >      _connections;
        // std::map<int, int>                  _server;
};
                               

#endif