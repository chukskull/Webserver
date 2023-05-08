#ifndef SERVER_HPP
# define SERVER_HPP

#include "headers.hpp"
#include "methods.hpp"
#include <map>

int _gl_recv_return;
class Server {
	
public:
	Server(std::vector<DataConf> &data):_containers(data) {
	}
		static bool isHexadecimal(const std::string& str)
		{
    	for (size_t i = 0; i < str.length() - 2; ++i)
		{
    	    if (!isxdigit(str[i]))
    	        return false;
    	}
   	 	return true;
	}
	static void    handle_chunked(Client &my_client)
	{
		std::string line;
		std::string fusil("");
		std::string	replace;
		int chunkSize = 0;
		std::stringstream	temp(my_client.get_buffer());
		// my_client._buffer->str("");
		while (std::getline(temp, line))
		{
			line.push_back('\n');
		if (isHexadecimal(line))
			{
				std::stringstream ss(line);
				std::cerr << line << std::endl;
				ss >> std::hex >> chunkSize;
				if (chunkSize == 0)
					my_client._done = true;
			}
		}
	}

	static int	receiving(int fd, Client &_my_client, char *buff)
	{
		std::string	sure;
		buff[BUFFER_SIZE + 1] = '\0';
			bzero(buff, BUFFER_SIZE);
			_gl_recv_return = recv(fd, buff, BUFFER_SIZE, 0);
			buff[_gl_recv_return] = '\0';
			std::cerr << buff << std::endl;
			if(_gl_recv_return > 0)
			{
				sure.append(buff);
				_my_client.AppendData(sure);
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
		return 0;
	}
	static void	_pars_request(Client &_my_client)
	{
		_string						line;
		std::stringstream			for_read(_my_client._buffer->str());
		std::string					header_check;
		bool						ok = false;

		for_read.seekg(0, std::ios::beg);
		for
		// while(getline(for_read,  line))
		// {
		// 	// line += '\n';
		// 	std::cerr << line << std::endl;
		// 	getchar();
		// 	if (line.find("Content-Length:") != _string::npos)
		// 	{
		// 		_string						str;
		// 		str = line.substr(line.find_first_of(" ") + 2);
		// 		std::stringstream convert;
		// 		convert << str;
		// 		convert >> _my_client.is_it_chunked_;
		// 		std::cerr << _my_client.is_it_chunked_ << "size contentleng " << std::endl;
		// 		// print_error << std::stoi(str) << std::endl;
		// 		break ;
		// 	}
		// 	else if (line.find("Transfer-Encoding:") != _string::npos)
		// 	{
		// 		puts("here");
		// 		_my_client.is_it_chunked_ = 1;
		// 		_string	str = line.substr(line.find("chunked"));
		// 		if (str.compare("chunked"))
		// 		{
		// 			_my_client._done = false;
		// 			ok = true;
		// 			_my_client.is_it_chunked_ = -1;
		// 				break ;
		// 		}
		// 	}
			// else if (line.find("Accept-Encoding:") != _string::npos)
			// {
			// 	_my_client.is_it_chunked_ = 1;
			// 	_my_client._done = true;
			// 	break;
			// }
		}
		// 	else if (line.find("Accept-Encoding:") != _string::npos)
		// 	{
		// 		// _string						str;
		// 		// str = line.substr(line.find_first_of(" ") + 2);
		// 		// std::stringstream convert;
		// 		// convert << str;
		// 		// convert >> _my_client.is_it_chunked_;
		// 		_my_client.is_it_chunked_ = 1;
		// 		if(ok == false)
		// 		{
		// 			_my_client._done = true;
		// 			// getchar();
		// 			break ;

		// 		}

		// 	// }
		// }
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
					{
						this->server_fds[server_fd] = std::make_pair(i, j);
						std::cerr << i << " " << j << std::endl;

					}
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
				if (listen(server_fd, 100000) == -1)
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

	// static void print_locations(std::vector<ReqLoc> &vec)
	// {

	// 	std::cout << "locs== \n";
	// 	for (std::vector<ReqLoc>::iterator it = vec.begin(); it != vec.end(); it++)
	// 	{
	// 		std::cout << "loc== ";
	// 		std::cout << "file: " << it->__file;
	// 		std::cout << " path: " << it->__path;
	// 		std::cout << " root: " << it->__root << std::endl;
	// 	}
	// }
	static	std::pair<int , int>	get_server_infos(std::map<int, std::pair<int, int> > &servers, int fd, std::map<int, Client> &concts)
	{
		int srvr = concts[fd].server_file;
		std::map<int, std::pair<int, int> >::iterator	it = servers.find(srvr);
		return it->second;
	}

   static void run(std::vector<DataConf> &__vec_data)
   {
		handler handl_request;
		deque<Mesage*> messages;
		lib.set(__vec_data);

		Server  ser(__vec_data);
		try
		{
			ser.initial_server(__vec_data);
		}
		catch(const std::string e)
		{
			std::cerr << e << '\n';
			exit(1);
		}

		while (1)
		{
			// ser.rc = 1;
			ser.rc = poll(&ser.fd_s[0], ser.fd_s.size(), -1);
			if (ser.rc < 0)
			{
				perror("poll() failed");
				break ;
			}
			if (ser.rc == 0)
			{
				std::cerr << "poll() timed out , End program\n" << std::endl;
				break ;
			}
			for (size_t i = 0; i < ser.fd_s.size(); i++)
			{
				if (ser.fd_s[i].revents & POLLIN || (ser.fd_s[i].revents & POLLOUT))
				{
					// std::map<int, pair<int, int>>::iterator	find_;
					if (ser.server_fds.find(ser.fd_s[i].fd) != ser.server_fds.end())
					{
						ser.addrlen = sizeof ser.remoteaddr;
						
						// std::cerr << "fds " << index << " " << ser.server_fds.size() << std::endl;
						int newfd = accept(ser.fd_s[i].fd, (struct sockaddr *) &ser.remoteaddr, &ser.addrlen);
						if (newfd == -1)
							perror("accept");
						else
						{
							ser.fd_s.push_back(pollfd());
							ser.fd_s.back().fd = newfd;
							ser.fd_s.back().events = POLLIN;
							ser.fd_counts++;
							// ser._connections.insert(std::make_pair(newfd, Client()));
							ser._connections[newfd] = Client(ser.fd_s[i].fd);
						}
							puts("here");
					} 
					else
					{
						std::pair<int, int> server_infos;
						server_infos = get_server_infos(ser.server_fds, ser.fd_s[i].fd, ser._connections);
						string res;

						int	_pars_req = receiving(ser.fd_s[i].fd, ser._connections[ser.fd_s[i].fd], ser.buf);
						if(_gl_recv_return > 0)
						{
							if (_pars_req)
							{
								_pars_request(ser._connections[ser.fd_s[i].fd]);
								if (ser._connections[ser.fd_s[i].fd].is_it_chunked_ == 0)
									perror("request header is not set corretely");
							}
							if (ser._connections[ser.fd_s[i].fd].is_it_chunked_ == -1)
							{
								handle_chunked(ser._connections[ser.fd_s[i].fd]);
							}
							else if(static_cast<size_t>(ser._connections[ser.fd_s[i].fd].is_it_chunked_) > ser._containers[server_infos.first].__body_size)
							{
								std::cerr << ser._connections[ser.fd_s[i].fd].get_buffer();
								perror("length size so big");
							}

								std::cerr <<  "index server "<< ser._connections[ser.fd_s[i].fd].server_file << std::endl;
								std::cerr <<"for length"<< ser._connections[ser.fd_s[i].fd].is_it_chunked_ << '\t' << ser._containers[server_infos.first].__body_size;
							if (ser._connections[ser.fd_s[i].fd]._done)
							{
								Mesage  *mesg = new Mesage();
								// string res;
								mesg->message = ser._connections[ser.fd_s[i].fd].get_buffer();
								ser._connections[ser.fd_s[i].fd].clear_buffer();
								
								
								
								mesg->_connections = std::make_pair(ser.fd_s[i].fd, server_infos);
								std::cerr << "server : " << server_infos.first << " port : " << server_infos.second << std::endl;
							// need to fill the sockets values in the response

								handl_request.handle(*mesg);
								messages.push_back(mesg);
								// std::cout << mesg->response << std::endl;
								// print_error << mesg->message << std::endl;
								// std::cerr << mesg->message.size() << std::endl;
								// print_error << "object send to ayman " <<mesg->_connections.first << " " <<mesg->_connections.second << std::endl;
							}
						}
						int sender_fd = ser.fd_s[i].fd;
						if (_gl_recv_return <= 0)
						{
							if (_gl_recv_return == 0) 
								std::cerr << "this client hung up " << sender_fd<< std::endl;
							else
							{
								//ayman needs to send the client 404 bad request in this
								puts("sure");
								perror("recv");
							}
							close(ser.fd_s[i].fd);
							ser.fd_s[i] = ser.fd_s.back();
							ser._connections.erase(ser.fd_s[i].fd);
							ser.fd_s.pop_back();
						}
						else
						{
							Mesage *mesg;
							if (!messages.empty())
							{
								mesg = messages.front();
								
								std::cout << mesg->message << std::endl;
								std::cout << mesg->response << std::endl;
								if(send(mesg->_connections.first, mesg->response.c_str(), mesg->response.length(), 0) < 0)
									perror("send");
								messages.pop_front();
								close(mesg->_connections.first);
							}
							// print_error <<" "<< pair_found.first << std::endl;
							// this part gonna send back the response to a client
							// close(pair_found.first);
						}
					}
					continue;
				}
			}
		}
	}
		std::map<int, std::pair<int, int> >		server_fds;
		int										rcv;
		int										on;
		int										rc;
		int										nfds;
		int										timeout;
		int										fd_counts;
		int										current_size;
		struct	addrinfo						hints, *ai, *point;
		struct	sockaddr_storage				remoteaddr;
		socklen_t 								addrlen;
		char									buf[BUFFER_SIZE + 1];
		_vector_fd	                        	fd_s;
		std::vector<DataConf>                      _containers;
		std::map<int, Client >					_connections;
};
							   

#endif