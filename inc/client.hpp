#ifndef CLIENT_HPP
#define CLIENT_HPP


#include "headers.hpp"


class Client {
	public:
		Client(size_t   _file):server_file(_file)
		{
			this->is_it_chunked_ = 0;
			_buffer = new std::stringstream();
			this->_size = 0;
			this->_done = true;
			this->_header_done = false;
			
		}
		Client()
		{
			// this->is_it_chunked_ = 0;
			// _buffer = new std::stringstream();
			// this->_size = 0;
			// this->_done = false;
			// this->_header_done = false;
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
		int	 						is_it_chunked_;
		size_t						_size;
		bool  						_done;
		bool  						_header_done;
};


#endif