#ifndef LOCATION_HPP
# define LOCATION_HPP

#include "headers.hpp"

class Location
{
	private:
		_string							__path;
		_string							__autoindex;
		_string							__place;
		_string							__root;
		_string							__name;
		_methods						__methods;
		_string							__cgi_path;
		_string							__cgi_ext;
		bool							_cgi;
		std::pair<bool, std::string>	__redirect;
	public:
		Location(_string path, _string autoindex, _string place_index, _string root, _methods const &meth, bool cgi, _string	cgi_path, _string cgi_ext, std::pair<bool, std::string> redre): __autoindex(autoindex),__methods(meth)
		{
			this->__path = path;
			this->_cgi = cgi;
			this->__cgi_path = cgi_path;
			this->__cgi_ext = cgi_ext;
			// this->__autoindex = autoindex;
			this->__place = place_index;
			this->__root = root;
			this->__redirect = redre;
		}
		_string	get_path()const{
			return __path;
		}
		_string	get_auto_index()const{
			return __autoindex;
		}
		_string	get_place()const{
			return __place;
		}
		_string	get_root()const{
			return __root;
		}
		_string	get_cgi_path()const{
			return __cgi_path;
		}
		_string	get_cgi_ext()const{
			return __cgi_ext;
		}
		bool get_cgi_bool()const
		{
			return	_cgi;
		}
		const _methods	get_methods()const{
			return __methods;
		}
		const std::pair<bool, _string> &get_redirect()const
		{
			return __redirect;
		}
};



#endif