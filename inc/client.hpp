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
			this->send_size = 0;
			this->lastActiveTime = -1;
			this->time_ = false;
			time(&lastActiveTime);
			
		}
		Client()
		{
			// this->is_it_chunked_ = 0;
			// _buffer = new std::stringstream();
			// this->_size = 0;
			// this->_done = false;
			// this->_header_done = false;
		}

		void	clear()
		{
			this->is_it_chunked_ = false;
			// if (_buffer != NULL)
			// {
			// 	delete _buffer;
			// 	_buffer = new std::stringstream();
			// }
			// else
			// 	_buffer = new std::stringstream();
			this->_size = 0;
			this->current_size = 0;
			this->_done = false;
			this->_header_done = false;
			this->header_size = 0;
			this->send_size = 0;
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
			// delete _buffer;
		};
		std::stringstream			*_buffer;
		int							server_file;
		std::pair<int, int>			host_src;
		bool	 					is_it_chunked_;
		bool						time_flag;
		size_t						_size;
		size_t						current_size;
		size_t						header_size;
		size_t						send_size;
		bool  						_done;
		bool						send_done;
		bool  						_header_done;
		_string						response;
		time_t 						lastActiveTime;
		bool						time_;
};


#endif