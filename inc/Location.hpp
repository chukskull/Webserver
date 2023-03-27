#ifndef LOCATION_HPP
# define LOCATION_HPP

#include "headers.hpp"

class Location
{
	private:
		_string							__path;
		_string                       __autoindex;
		_string                       __place;
		_string                       __root;
		_methods                      __methods;
		std::pair<bool, std::string>  __redirect;
	public:
		Location(_string path, _string autoindex, _string place_index, _string root, _methods const &meth, std::pair<bool, std::string> redre): __methods(meth)
		{
			this->__path = path;
			this->__autoindex = autoindex;
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
		const _methods	&get_methods()const{
			return __methods;
		}
		const std::pair<bool, _string> &get_redirect()const
		{
			return __redirect;
		}
};



#endif