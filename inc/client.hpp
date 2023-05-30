#ifndef CLIENT_HPP
#define CLIENT_HPP


#include "headers.hpp"


class Client {
	public:
		Client(size_t   _file):server_file(_file)
		{
			this->is_it_chunked_ = false;
			_buffer = new std::stringstream();
			this->_size = 0;
			this->current_size = 0;
			this->_done = false;
			this->_header_done = false;
			this->header_size = 0;
			this->lastActiveTime = -1;
			time(&lastActiveTime);
			
		}
		Client(){};
		void	clear()
		{
			this->is_it_chunked_ = false;
			delete _buffer;
			this->_size = 0;
			this->current_size = 0;
			this->_done = false;
			this->_header_done = false;
			this->header_size = 0;
		}
		void AppendData(const std::string& data)
		{
			(*_buffer) << data; // Use the * operator to dereference the pointer and append data
		}
		std::string get_buffer() const
		{
			return (*_buffer).str();
		}
		void clear_buffer()
		{
			(*_buffer).str("");
		}

		~Client(){
		};
		std::stringstream			*_buffer;
		int							server_file;
		std::pair<int, int>			host_src;
		bool	 					is_it_chunked_;
		bool						time_flag;
		size_t						_size;
		size_t						current_size;
		size_t						header_size;
		bool  						_done;
		bool  						_header_done;
		_string						response;
		time_t 						lastActiveTime;
};


#endif