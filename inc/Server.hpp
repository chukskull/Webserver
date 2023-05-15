#ifndef SERVER_HPP
# define SERVER_HPP

#include "headers.hpp"
#include "methods.hpp"
#include <map>

int 	_gl_recv_return;

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
	static bool		decoding_chunked(Client &my_client, _string	&temp)
	{
		std::string chunk_size_str;
    	int chunk_size = 0;
    	bool read_chunk_size = true;
		_string	_header = temp.substr(0, my_client.header_size);
		_string _body = temp.substr(my_client.header_size);
		std::stringstream _clean_body;
		// //std::cerr << _body << std::endl;
		(*my_client._buffer).str("");
		(*my_client._buffer).clear();

		//std::cerr << "BUFFEr" << my_client.get_buffer() << std::endl;
		size_t n = _body.size();
		std::cerr << "body size : "<<_body.size() << std::endl;
		for (size_t i = 0; i < n; i++)
        {
            if (read_chunk_size)
            {
                if (_body[i] == '\r' && _body[i + 1] == '\n')
                {
					if (chunk_size_str.size() == 0)
						continue;
					// for(size_t k= 0 ; k < chunk_size_str.size(); k++)
					// {
					// 	std::cerr << (int) chunk_size_str[k] << std::endl;
					// }
					// chunk_size = 0;
					chunk_size = strtol(chunk_size_str.c_str(), NULL, 16);
					if (chunk_size_str == "0\r\n\r\n")
					{
						break ;
					}
                    chunk_size_str.clear();
                    read_chunk_size = false;
					i++; // Skip '\n'
                }
                else
                {
                    chunk_size_str += _body[i];
                }
				
            }
            else
            {
                chunk_size--;
				std::cerr << chunk_size << std::endl;
                _clean_body.write(&_body[i], 1);
                if (chunk_size == 0)
                {
                    read_chunk_size = true;
                }
            }
		}
		// _string	_clen(_clean_body.str());
		
		// std::cerr << _clean_body.str() << std::endl;
		// std::cerr << _header << std::endl;
		// std::cerr <<"clean" <<_clean_body.str().size() << " " <<_clen.substr(0,200)<< " <---- head" << std::endl;
		(*my_client._buffer).write((_header + _clean_body.str()).c_str(), (_header + _clean_body.str()).size());
		// std::cerr << (*my_client._buffer).str() << std::endl;
		return 1;
	}
	static void    handle_chunked(Client &my_client)
	{
		// std::string line;
		// std::string fusil("");
		// std::string	replace;
		// int chunkSize = 0;
		bool	error;
		_string	temp(my_client.get_buffer());
		size_t	test_temp;
		std::cerr << "here  " << temp.size() << std::endl;
		if ((test_temp = temp.find("0\r\n\r\n")) != _string::npos)
		{
			if (temp[test_temp - 1] == '\n' && temp[test_temp - 2] == '\r')
				my_client._done = true;
			// std::cerr << "finished" << std::endl;
			std::cerr << temp.size() << " " <<my_client.get_buffer().size() << "checiking size ok " << std::endl;
		}
		if (my_client._done)
		{
			error = decoding_chunked(my_client, temp);
			std::cerr << "here okey " << std::endl;
		}
		// my_client._buffer->str("");
		// while (std::getline(temp, line))
		// {
		// 	line.push_back('\n');
		// if (isHexadecimal(line))
		// 	{
		// 		std::stringstream ss(line);
		// 		//std::cerr << line << std::endl;
		// 		ss >> std::hex >> chunkSize;
		// 		if (chunkSize == 0)
		// 			my_client._done = true;
		// 	}
		// }
	}

	static int	receiving(int fd, Client &_my_client, char *buff2)
	{
			std::string	sure;
			(void)buff2;
			char buff[BUFFER_SIZE];
			bzero(buff, BUFFER_SIZE);

			_gl_recv_return = read(fd, buff, BUFFER_SIZE);
			// std::cerr << buff << " " << strlen(buff) << std::endl;
			// getchar();
			if(_gl_recv_return > 0)
			{
				// sure.append(buff);
				
				// buff[_gl_recv_return + 1] = '\0';
				// //std::cerr << buff << std::endl;
				//std::cerr << _string(buff).size() << std::endl;
				(*_my_client._buffer).write(buff, _gl_recv_return);
				_my_client.current_size += _gl_recv_return;
				//std::cerr << "buffer of my clinet \t" << _my_client.get_buffer().size() <<'\t' <<_my_client.current_size<< "return "<< _gl_recv_return << std::endl;
				if(_my_client._header_done)
					return 0;
				_string	line;
				std::istringstream	obj(buff);
				while (getline(obj, line))
				{
					size_t j = 0;
					line += "\n";
					_my_client.header_size += line.size();
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
		size_t							index = 0;
		std::string					header_check;
		_string						str;
		size_t						endindex;

		header_check  = _my_client._buffer->str();
		if ((index = header_check.find("Content-Length: ")) != _string::npos)
		{
			index += 16;
			endindex= header_check.find("\r\n", index);
			str = header_check.substr(index, endindex - index);
			_my_client._size = atoi(str.c_str());
		}
		else if ((index = header_check.find("Transfer-Encoding: ")) != _string::npos)
		{
			index += 19;
			endindex = header_check.find("\r\n", index);
			str = header_check.substr(index, endindex - index);
			if(str == "chunked")
			{
				_my_client._done = false;
				_my_client.is_it_chunked_= true;
			}
			else
				_my_client._done = true;
		}
		else if (header_check.find("Accept-Encoding:") != _string::npos)
		{

			_my_client._done = true;
		}
		else
			perror("request header is not set corretely");
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
						this->server_fds[server_fd] = std::make_pair(i, j);
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
	static	int	send_response(Client &my_client, int fd)
	{
			ssize_t		s;
			if (my_client.response.size())
			{
				if (my_client.response.size() > BUFFER_SEND)
				{
					s = send(fd ,my_client.response.c_str(), BUFFER_SEND, 0);
					if (s < 0)
						return -1;
					if (s > 0)
						my_client.response.erase(0, 1200);
				}
				else
				{
					s = send(fd, my_client.response.c_str(), my_client.response.length(), 0);
					if (s > 0)
						my_client.response.clear();

					else
						return -1;

				}
				//std::cerr << s  << '\t' << my_client.response.length() << std::endl;
				if(my_client.response.size() == 0)
				{
					// getchar();
					my_client.clear();
					close(fd);
				}
			}
			return 0;		
	}

   static void run(std::vector<DataConf> &__vec_data)
   {
		handler handl_request;
		// deque<Mesage*> messages;
		lib.set(__vec_data);

		Server  ser(__vec_data);
		try
		{
			ser.initial_server(__vec_data);
		}
		catch(const std::string e)
		{
			//std::cerr << e << '\n';
			exit(1);
		}

		while (true)
		{
			ser.rc = poll(&ser.fd_s[0], ser.fd_s.size(), -1);
			if (ser.rc < 0)
			{
				perror("poll() failed");
				break ;
			}
			if (ser.rc == 0)
			{
				//std::cerr << "poll() timed out , End program\n" << std::endl;
				break ;
			}
			for (size_t i = 0; i < ser.fd_s.size(); i++)
			{
				if (ser.fd_s[i].revents & POLLIN)
				{
					// std::cerr << "waiting..." << std::endl;
					if (ser.server_fds.find(ser.fd_s[i].fd) != ser.server_fds.end())
					{
						ser.addrlen = sizeof ser.remoteaddr;
						int newfd = accept(ser.fd_s[i].fd, (struct sockaddr *) &ser.remoteaddr, &ser.addrlen);
						if (newfd == -1)
							perror("accept");
						else
						{
							//std::cerr << ser.fd_s[i].fd << std::endl;
							//std::cerr << "dima new client ??" << std::endl;
							ser.fd_s.push_back(pollfd());
							ser.fd_s.back().fd = newfd;
							ser.fd_s.back().events = POLLIN;
							ser.fd_counts++;
							ser._connections[newfd] = Client(ser.fd_s[i].fd);
						}
					}
					else
					{
						// std:: cerr << "client phase " <<ser.fd_s[i].fd << std::endl;
						std::pair<int, int> server_infos;
						server_infos = get_server_infos(ser.server_fds, ser.fd_s[i].fd, ser._connections);
						string res;

						int	_pars_req = receiving(ser.fd_s[i].fd, ser._connections[ser.fd_s[i].fd], ser.buf);
						if(_gl_recv_return > 0)
						{
							if (_pars_req)
								_pars_request(ser._connections[ser.fd_s[i].fd]);

							if (ser._connections[ser.fd_s[i].fd].is_it_chunked_)
								handle_chunked(ser._connections[ser.fd_s[i].fd]);
							
							else if (ser._connections[ser.fd_s[i].fd]._size == (ser._connections[ser.fd_s[i].fd].current_size - ser._connections[ser.fd_s[i].fd].header_size))
								ser._connections[ser.fd_s[i].fd]._done = true;

							// if(static_cast<size_t>(ser._connections[ser.fd_s[i].fd]._size) > ser._containers[server_infos.first].__body_size)
							// {
							// 	perror("length size so big");
							// }
							
							if (ser._connections[ser.fd_s[i].fd]._done)
							{
								Mesage  *mesg = new Mesage();
								//std::cerr <<  ser._connections[ser.fd_s[i].fd].get_buffer().size() << '\t' <<ser._connections[ser.fd_s[i].fd]._size << '\t' <<  (ser._connections[ser.fd_s[i].fd].current_size - ser._connections[ser.fd_s[i].fd].header_size) << std::endl;
								mesg->message = ser._connections[ser.fd_s[i].fd].get_buffer();
								// std::cerr << mesg->message.size() -  ser._connections[ser.fd_s[i].fd].header_size<< "PURITY" << std::endl;
								ser._connections[ser.fd_s[i].fd].clear_buffer();
								mesg->_connections = std::make_pair(ser.fd_s[i].fd, server_infos);
								// //std::cerr << "server : " << server_infos.first << " port : " << server_infos.second << '\t' <<server_infos.first <<std::endl;
								std::cout << ":::" << mesg->message.size() << std::endl;
								// std::cout << ser._connections[ser.fd_s[i].fd]._size << std::endl;
								// std::cout << ":-:" << mesg->message.substr(0, 400) << std::endl;
								handl_request.handle(*mesg);
								
								ser._connections[ser.fd_s[i].fd].response.swap(mesg->response);
								ser.fd_s[i].events = POLLOUT;
							}
						}
						int sender_fd = ser.fd_s[i].fd;
						//std::cerr << "here client" << std::endl;
						if (_gl_recv_return <= 0)
						{
							//std::cerr << "pretty sure " << std::endl;
							if (_gl_recv_return == 0) 
								std::cerr << "this client hung up " << sender_fd<< std::endl;
							else
							{
								//ayman needs to send the client 404 bad request in this
								
								perror("recv");
							}
							close(ser.fd_s[i].fd);
							ser.fd_s[i] = ser.fd_s.back();
							ser._connections.erase(ser.fd_s[i].fd);
							ser.fd_s.pop_back();
						}
					}
				}
				else if (ser.fd_s[i].revents & POLLOUT)
				{
					ssize_t 	s;
					s = send_response(ser._connections[ser.fd_s[i].fd], ser.fd_s[i].fd);
					if (s < 0)
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
		_vector_fd								fd_s;
		std::vector<DataConf>					_containers;
		std::map<int, Client >					_connections;
};
							   

#endif