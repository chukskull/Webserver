#ifndef SERVER_HPP
# define SERVER_HPP

#include "headers.hpp"
#include "methods.hpp"
#include <map>
#include <set>

#define	ClienTsoCKet 	ser.fd_s[i].fd
#define MyClienT 		ser._connections[ClienTsoCKet]

int 	_gl_recv_return;

class Server {
	
public:
	Server(std::vector<DataConf> &data):_containers(data) {
	}

	static bool		decoding_chunked(Client &my_client, _string	&temp, size_t body_size)
	{
		std::string 		chunk_size_str;
		std::stringstream 	_clean_body;
    	int chunk_size = 0;
    	bool read_chunk_size = true;
		_string	_header = temp.substr(0, my_client.header_size);
		_string _body = temp.substr(my_client.header_size);
		(*my_client._buffer).str("");
		(*my_client._buffer).clear();

		size_t n = _body.size();
		for (size_t i = 0; i < n; i++)
        {
            if (read_chunk_size)
            {
                if (_body[i] == '\r' && _body[i + 1] == '\n')
                {
					if (chunk_size_str.size() == 0)
						continue;
					chunk_size = strtol(chunk_size_str.c_str(), NULL, 16);
					if (chunk_size > static_cast<int>(body_size))
						return false;
					if (chunk_size_str == "0\r\n\r\n")
						break ;
                    chunk_size_str.clear();
                    read_chunk_size = false;
					i++;//skip n hh
                }
                else
                    chunk_size_str += _body[i];
            }
            else
            {
                _clean_body.write(&_body[i], chunk_size);
                read_chunk_size = true;
                i += chunk_size;
				i--;
            }
		}
		(*my_client._buffer).write((_header + _clean_body.str()).c_str(), (_header + _clean_body.str()).size());
		return 1;
	}

	static bool    handle_chunked(Client &my_client, size_t body_size)
	{
		bool	error;
		handler						body;
		_string	temp(my_client.get_buffer());
		size_t	test_temp;
		bool		done = false;

		if ((test_temp = temp.find("0\r\n\r\n")) != _string::npos)
		{
			if (temp[test_temp - 1] == '\n' && temp[test_temp - 2] == '\r')
				done = true;
		}
		if (done)
		{
			error = decoding_chunked(my_client, temp, body_size);
			if (!error)
			{
				return false;
			}
			else
				my_client._done = true;
		}
		return true;
	}

	static int	receiving(int fd, Client &_my_client)
	{
			std::string	sure;
			char buff[BUFFER_SIZE];
			bzero(buff, BUFFER_SIZE);

			_gl_recv_return = read(fd, buff, BUFFER_SIZE);
			if(_gl_recv_return > 0)
			{
				time(&_my_client.lastActiveTime);
				(*_my_client._buffer).write(buff, _gl_recv_return);
				_my_client.current_size += _gl_recv_return;
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
	static int pars_request(Client &_my_client)
	{
		size_t							index = 0;
		std::string					header_check;
		_string						str;
		size_t						endindex;
		handler						header;

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
		{
			header.manage_server_errors(400, _my_client.response);
			return -1;
		}
		return 0;
	}

	static std::vector<ForBind>		for_binding_servers(std::vector<DataConf> &data)
	{
		std::vector<ForBind>	vec;
		std::map<string, std::vector<string> >		sure;
		std::vector<string>		ko;
		

		for (size_t i  = 0; i < data.size(); i++)
		{
			for(size_t j = 0;j < data[i].__port.size(); j++)
			{
				ko.push_back(data[i].__port[j]);
				sure[data[i].__host] = ko;
			}
		}
		std::map<string, std::vector<string> >::iterator	it = sure.begin();
		for(;it != sure.end(); it++)
		{
			ForBind	ip_port;
			ip_port.server = it->first;
			for(size_t	i = 0; i < it->second.size(); i++)
			{
				if (std::find(ip_port._Port.begin(), ip_port._Port.end(), it->second[i]) == ip_port._Port.end())
					ip_port._Port.push_back(it->second[i]);
			}
			vec.push_back(ip_port);
		}
		return vec;
	}
	void	initial_server(std::vector<DataConf> &data)
	{
		bzero(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;
		std::vector<ForBind>	_for_bind;
	
		_for_bind = for_binding_servers(data);
		
		for(size_t  i = 0; i < _for_bind.size(); i++)
		{
			for(size_t j = 0; j < _for_bind[i]._Port.size(); j++)
			{
				int server_fd;
				print_error << _for_bind[i]._Port[j].c_str() << std::endl;
				if ((rcv = getaddrinfo(_for_bind[i].server.c_str() , _for_bind[i]._Port[j].c_str(), &hints, &ai)) != 0) {
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
				if (listen(server_fd, SOMAXCONN) == -1)
					throw std::string("problem with listen");
				else
					print_error << server_fd << std::endl;
				pollfd  temp;
				temp.fd = server_fd;
				temp.events = POLLIN;
				fd_s.push_back(temp);
			}
		}
	}

	~Server() {
		
	}

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
						my_client.response.erase(0, BUFFER_SEND);
				}
				else
				{
					s = send(fd, my_client.response.c_str(), my_client.response.size(), 0);
					if (s > 0)
						my_client.response.clear();
					else
						return -1;

				}
				if(my_client.response.size() == 0)
				{
					my_client.clear();
					usleep(200);
					close(fd);
					return 2;
				}
			}
			return 0;		
	}

   static void run(std::vector<DataConf> &__vec_data)
   {
		handler handl_request;
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
		while (true)
		{
			ser.rc = poll(&ser.fd_s[0], ser.fd_s.size(), 5000);
			if (ser.rc < 0)
			{
				perror("poll() failed");
				break ;
			}
			for (size_t i = 0; i < ser.fd_s.size(); i++)
			{
				if (ser.fd_s[i].revents & POLLIN)
				{
					if (ser.server_fds.find(ClienTsoCKet) != ser.server_fds.end())
					{
						ser.addrlen = sizeof ser.remoteaddr;
						int newfd = accept(ClienTsoCKet, (struct sockaddr *) &ser.remoteaddr, &ser.addrlen);
						if (newfd == -1)
						{
							perror("accept");
						}
						else
						{
							ser.fd_s.push_back(pollfd());
							ser.fd_s.back().fd = newfd;
							ser.fd_s.back().events = POLLIN;
							ser._connections[newfd] = Client(ClienTsoCKet);
						}
					}
					else
					{
						MyClienT.time_flag = true;
						std::pair<int, int> server_infos;
						int			error_header;
						server_infos = get_server_infos(ser.server_fds, ClienTsoCKet, ser._connections);
						string res;

						int	_pars_req = receiving(ClienTsoCKet, MyClienT);
						if(_gl_recv_return > 0)
						{
							if (_pars_req)
							{
								error_header = pars_request(MyClienT);
								if (error_header == -1)
									ser.fd_s[i].events = POLLOUT;
							}

							if (MyClienT.is_it_chunked_)
							{
								if (!handle_chunked(MyClienT, ser._containers[server_infos.first].__body_size))
								{
									handl_request.manage_server_errors(413, MyClienT.response);
									ser.fd_s[i].events = POLLOUT;
								}
							}
							
							else if (MyClienT._size == (MyClienT.current_size - MyClienT.header_size))
								MyClienT._done = true;

							if(static_cast<size_t>(MyClienT._size) > ser._containers[server_infos.first].__body_size)
							{
								handl_request.manage_server_errors(413, MyClienT.response);
								// MyClienT.time_ = true;
								ser.fd_s[i].events = POLLOUT;
							}
							
							if (MyClienT._done)
							{
								Mesage  *mesg = new Mesage();
								mesg->message = MyClienT.get_buffer();
								MyClienT.clear_buffer();
								mesg->_connections = std::make_pair(ClienTsoCKet, server_infos);
								std::cout << "got request from client " << std::endl;
								handl_request.handle(*mesg);
								MyClienT.response.swap(mesg->response);
								ser.fd_s[i].events = POLLOUT;
							}
						}
						int sender_fd = ClienTsoCKet;
						if (_gl_recv_return <= 0)
						{
							if (_gl_recv_return == 0)
							{
								std::cerr << "this client hung up " << sender_fd<< std::endl;
								close(ClienTsoCKet);
								ser.fd_s[i] = ser.fd_s.back();
								ser._connections.erase(ClienTsoCKet);
								ser.fd_s.pop_back();
							}
							else
							{
								handl_request.manage_server_errors(500, MyClienT.response);
								ser.fd_s[i].events = POLLOUT;
							}
						}
					}
				}
				else if (ser.fd_s[i].revents & POLLOUT)
				{
					ssize_t 	s;
					MyClienT.time_flag = false;
					s = send_response(MyClienT, ClienTsoCKet);
					if (s == 2)
						ser.fd_s.erase(ser.fd_s.begin() + i);
					if (s < 0)
						continue;
				}
			}		

        	for (size_t i = 0; i < ser.fd_s.size(); ++i)
        	{
				if(MyClienT.time_flag)
				{
					time_t currentTime;
        			time(&currentTime);
        	        if (currentTime - MyClienT.lastActiveTime > 10 && ser.server_fds.find(ClienTsoCKet) == ser.server_fds.end())
        	        {
						handl_request.manage_server_errors(408, MyClienT.response);
						MyClienT.time_ = true;
						ser.fd_s[i].events = POLLOUT;
        	        }
				}
        	}
		}
	}
		std::map<int, std::pair<int, int> >		server_fds;
		int										rcv;
		int										on;
		int										rc;
		int										nfds;
		int										current_size;
		struct	addrinfo						hints, *ai, *point;
		struct	sockaddr_storage				remoteaddr;
		socklen_t 								addrlen;
		_vector_fd								fd_s;
		std::vector<DataConf>					_containers;
		std::map<int, Client >					_connections;
};
							   

#endif