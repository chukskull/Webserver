#ifndef SERVER_HPP
# define SERVER_HPP

#include "headers.hpp"
#include <map>


int _gl_recv_return;
class Server {
	
public:
	Server(std::vector<DataConf> &data):_containers(data) {
	}
	
	static void    handle_chunked(Client &my_client)
	{
		std::string line;
		int chunkSize = 0;
		std::stringstream	temp(my_client.get_buffer());
		while (std::getline(temp, line))
		{
		   	if (isxdigit(line[0]))
			{
				std::stringstream ss(line);
				ss >> std::hex >> chunkSize;
				if (chunkSize == 0)
					my_client._done = true;
			}
		}
	}

	static int	receiving(int fd, Client &_my_client, char *buff)
	{
		std::string	sure;
		while (1)
		{
			bzero(buff, BUFFER_SIZE);
			_gl_recv_return = recv(fd, buff, BUFFER_SIZE, 0);
			sure.append(buff);
			if(_gl_recv_return > 0)
			{
				_my_client.AppendData(sure);
				_my_client._size = _my_client.get_buffer().size() - 1;
				if(_my_client._header_done)
					return 0;
				_string	line;
				std::istringstream	obj(sure);
				while (getline(obj, line))
				{
					size_t j = 0;
					line += "\n";
					if((j = line.find("\r\n")) != _string::npos)
					{
						if (j == 0)
						{
							_my_client._header_done = true;
							return 1;
						}
					}
				}
			}
			else
				break;
		}
		return 0;
	}
	static void	_pars_request(Client &_my_client)
	{
		_string						line;
		std::stringstream			for_read(_my_client._buffer->str());

		for_read.seekg(0, std::ios::beg);
		while(getline(for_read,  line))
		{
			if (line.find("Content-Length:") != _string::npos)
			{
				_string						str;
				str = line.substr(line.find_first_of(" ") + 2);
				std::stringstream convert;
				convert << str;
				convert >> _my_client.is_it_chunked_;
				_my_client._done = true;
				print_error << std::stoi(str) << std::endl;
				break ;
			}
			else if (line.find("Transfer-Encoding:") != _string::npos)
			{
				_string	str = line.substr(line.find("chunked"));
				if (str.compare("chunked"))
				{
					_my_client.is_it_chunked_ = -1;
						break ;
				}
			}
		}
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
				for (point = ai; point != NULL; point = point->ai_next)
				{
					server_fd = socket(point->ai_family, point->ai_socktype, point->ai_protocol);
					if (server_fd < 0)
						continue;
					else
						this->server_fds.push_back(server_fd);
					this->on = 1;
					rc = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
					if (rc < 0)
						throw std::string("error with setsocketopt");
					if (fcntl(server_fd, F_SETFL, O_NONBLOCK) < 0)
						throw std::string("error fcntl");
					if (bind(server_fd, point->ai_addr, point->ai_addrlen) < 0) {
						close(server_fd);
						continue;
					}
					break ;
				}
				freeaddrinfo(ai);
				if (point == NULL)
						throw std::string("there's no ip available in this host");
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

   static void run(std::vector<DataConf> &__vec_data)
   {
		Server  __my_ser(__vec_data);
		try
		{
			__my_ser.initial_server(__vec_data);
		}
		catch(const std::string e)
		{
			std::cerr << e << '\n';
			exit(1);
		}
		
		while (1) {
			__my_ser.rc = 1;
			__my_ser.rc = poll(&__my_ser.fd_s[0], __my_ser.fd_s.size(), __my_ser.timeout);
			if (__my_ser.rc < 0)
			{
				perror("poll() failed");
				break ;
			}
			if (__my_ser.rc == 0)
			{
				std::cerr << "poll() timed out , End program\n" << std::endl;
				break ;
			}
			for (size_t i = 0; i < __my_ser.fd_s.size(); i++)
			{
				if (__my_ser.fd_s[i].revents & POLLIN)
				{
					size_t                      index = 0;
					std::vector<int>::iterator  find_;
					if ((find_ = find(__my_ser.server_fds.begin(), __my_ser.server_fds.end(), __my_ser.fd_s[i].fd)) != __my_ser.server_fds.end())
					{
						__my_ser.addrlen = sizeof __my_ser.remoteaddr;
						index = __my_ser.server_fds[find_ - __my_ser.server_fds.begin()];
						int newfd = accept(__my_ser.server_fds[find_ - __my_ser.server_fds.begin()], (struct sockaddr *) &__my_ser.remoteaddr, &__my_ser.addrlen);
						if (newfd == -1)
							perror("accept");
						else
						{
							__my_ser.fd_s.push_back(pollfd());
							__my_ser.fd_s.back().fd = newfd;
							__my_ser.fd_s.back().events = POLLIN;
							__my_ser.fd_counts++;
							__my_ser._connections.insert(std::make_pair(newfd, Client(index)));
						}
					} 
					else
					{
						int	_pars_req = receiving(__my_ser.fd_s[i].fd, __my_ser._connections[__my_ser.fd_s[i].fd], __my_ser.buf);
						if(_gl_recv_return > 0)
						{
							if (_pars_req)
							{
								_pars_request(__my_ser._connections[__my_ser.fd_s[i].fd]);
								if (__my_ser._connections[__my_ser.fd_s[i].fd].is_it_chunked_ == 0)
									perror("request header is not set corretely");
							}
							if (__my_ser._connections[__my_ser.fd_s[i].fd].is_it_chunked_ == -1)
							{
									handle_chunked(__my_ser._connections[__my_ser.fd_s[i].fd]);

							}
							else if(static_cast<size_t>(__my_ser._connections[__my_ser.fd_s[i].fd].is_it_chunked_) > __my_ser._containers[__my_ser._connections[__my_ser.fd_s[i].fd]._host_src].__body_size)
								perror("length size so big");
						}
						Mesage  *mesg = new Mesage();
						int sender_fd = __my_ser.fd_s[i].fd;
						if (__my_ser._connections[__my_ser.fd_s[i].fd]._done)
						{
							mesg->message = __my_ser._connections[__my_ser.fd_s[i].fd].get_buffer();
							mesg->_connections = std::make_pair(__my_ser.fd_s[i].fd, __my_ser._connections[__my_ser.fd_s[i].fd]._host_src);
							print_error << mesg->message << std::endl;
							print_error << "object send to ayman " <<mesg->_connections.first << " " <<mesg->_connections.second << std::endl;
						}
						if (_gl_recv_return <= 0)
						{
							if (_gl_recv_return == 0) 
								std::cerr << "this client hung up " << sender_fd<< std::endl;
							else
								perror("recv");
							close(__my_ser.fd_s[i].fd);
							__my_ser.fd_s[i] = __my_ser.fd_s.back();
							__my_ser._connections.erase(__my_ser.fd_s[i].fd);
							__my_ser.fd_s.pop_back();
						}
					else
						{
							// print_error <<" "<< pair_found.first << std::endl;
							// this part gonna send back the response to a client
							// if(send(pair_found.first, "hello_there", 12, 0) < 0)
							// 	perror("send");
							// close(pair_found.first);
						}
					}
				}
			}
		}
	}
		std::vector<int>					        server_fds;
		int									        rcv;
		int									        on;
		int							                rc;
		int							                nfds;
		int									        timeout;
		int									        fd_counts;
		int									        current_size;
		struct	addrinfo					        hints, *ai, *point;
		struct	sockaddr_storage			        remoteaddr;
		socklen_t 							        addrlen;
		char								        buf[BUFFER_SIZE + 1];
		_vector_fd	                                fd_s;
		std::vector<DataConf>                       _containers;
		std::map<int, Client >      				_connections;
};
							   

#endif