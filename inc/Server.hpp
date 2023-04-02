
// #include "server.hpp"
// #include "ConfigParser.hpp"
#ifndef SERVER_HPP
# define SERVER_HPP
#include "headers.hpp"


class Server {
public:
    Server(DataConf &data):contain(data) {
    }
	void	initial_server(DataConf *data)
	{
        print_error << "am here" << std::endl;
        fd_s.resize(1);
        bzero(&hints, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;
        this->current_size = 1;
        print_error << "bouadel "<<data->__host.c_str() << std::endl;
        if ((rcv = getaddrinfo(data->__host.c_str() , data->__port.c_str(), &hints, &ai)) != 0) {
            throw std::string("error in getaddrinfo");
        }
        print_error << " cringe " << std::endl;
        for (point = ai; point != NULL; point = point->ai_next) {
            std::cerr << point->ai_addr << std::endl;
            server_fd = socket(point->ai_family, point->ai_socktype, point->ai_protocol);
            if (server_fd < 0)
                continue;
            this->on = 1;
            rc = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
            if (rc < 0) {
                // perror("setsocketopt failed");
                throw std::string("error with setsocketopt");
                // exit(-1);
            };
            if (bind(server_fd, point->ai_addr, point->ai_addrlen) < 0) {
                close(server_fd);
                continue;
            }
            break ;
        }
        freeaddrinfo(ai);
        if (point == NULL) {
			throw std::string("there's no ip available in this host");
            // std::cerr << "there's no ip available in this host" << std::endl;
            // return;
        }
        if (listen(server_fd, 10) == -1) {
            throw std::string("problem with listen");
        }
        int terminal_fd = open("ok.txt", O_CREAT | O_WRONLY, 0777);
        if (terminal_fd == -1)
            perror("fd");
        fd_s[0].fd = server_fd;
        fd_s[0].events = POLLIN;
        timeout = (2 * 60 * 1000);
        fd_counts = 1;
	}
    ~Server() {}

   static void run(DataConf *data) {
    print_error << "rijal" << std::endl;
        Server  __my_ser(*data);
		try
		{
			__my_ser.initial_server(data);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
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
            __my_ser.current_size = __my_ser.nfds;
            for (size_t i = 0; i < __my_ser.fd_s.size(); i++) {
                if (__my_ser.fd_s[i].revents & POLLIN) {
                    if (__my_ser.fd_s[i].fd == __my_ser.server_fd) {
                        __my_ser.addrlen = sizeof __my_ser.remoteaddr;
                        int newfd = accept(__my_ser.server_fd, (struct sockaddr *) &__my_ser.remoteaddr, &__my_ser.addrlen);
                        if (newfd == -1)
                            perror("accept");
                        else {
                            __my_ser.fd_s.push_back(pollfd());
                            __my_ser.fd_s.back().fd = newfd;
                            __my_ser.fd_s.back().events = POLLIN;
                            __my_ser.fd_counts++;
                        }
                    } else {
                        int bytes = recv(__my_ser.fd_s[i].fd, __my_ser.buf, BUFFER_SIZE, 0);
                        __my_ser.buf[bytes] = '\0';
                        std::string message(__my_ser.buf);
                        int sender_fd = __my_ser.fd_s[i].fd;
                        if (bytes <= 0) {
                            if (bytes == 0) 
							{
								std::cerr << "this client hung up " << sender_fd<< std::endl;

							}
							else
							{
								perror("rev");
							}
						
						close(__my_ser.fd_s[i].fd);
						__my_ser.fd_s[i] = __my_ser.fd_s.back();
						__my_ser.fd_s.pop_back();
					}
					else
						{
							//this part gonna send back the response to a client
							// write(terminal_fd, message.c_str(), message.length());	
						}
					}
				}
			}
		}
	}
		int									server_fd;
		int									rcv;
		int									on;
		int							        rc;
		int							        nfds;
		int									timeout;
		int									fd_counts;
		int									current_size;
		struct	addrinfo					hints, *ai, *point;
		struct	sockaddr_storage			remoteaddr;
		socklen_t 							addrlen;
		char								buf[BUFFER_SIZE + 1];
		_vector_fd	fd_s;
        struct DataConf                     contain;
};
                               

#endif