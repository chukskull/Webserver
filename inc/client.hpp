#ifndef CLIENT_HPP
#define CLIENT_HPP


#include "headers.hpp"


class Client {
    public:
        Client(size_t   _file):_host_src(_file)
        {
            this->is_it_chunked_ = 0;
            _buffer = new std::stringstream();
            this->_size = 0;
            this->_done = false;
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
        ~Client(){
            // delete _buffer;
        };
        std::stringstream           *_buffer;
        int                         _host_src;
        int                         is_it_chunked_;
        size_t                      _size;
        bool                        _done;
        bool                        _header_done;
};


#endif