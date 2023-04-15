#ifndef REQUESTCONFIG_HPP
#define REQUESTCONFIG_HPP

#include "headers.hpp"

typedef  std::vector<bool> MethAllow;

struct ReqLoc
{
	bool                        _autoindex;
	_string		                __path;
	// vector<_string>             __files;
	_string						__file;
	_string                     __root;
	MethAllow                   _AllowMeth;
	std::pair<bool, std::string> __redirect;


	//constructor for RecLoc that sets all the values to empty strings

	ReqLoc(): _autoindex(false), __path(""), __root(""), __redirect(false, "") , set(false){
		_AllowMeth.resize(3, false);
	}

	ReqLoc operator=(const ReqLoc &other)
	{
		this->_autoindex = other._autoindex;
		this->__path = other.__path;
		// this->__files = other.__files;
		this->__root = other.__root;
		this->__redirect = other.__redirect;
		set = true;
		this->_AllowMeth = other._AllowMeth;
		// for (size_t i = 0; i < 3; i++)
		// 	this->_AllowMeth[i] = other._AllowMeth[i];
		return *this;
	}
	
	bool is_set() {return set; }

	private:
		bool set;
};

struct DataConf
{
	size_t					__body_size;
	_string					__host;
	_string					__name;
	std::vector<_string>	__port;
	std::vector<ReqLoc>		__locations;
};

#endif