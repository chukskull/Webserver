#ifndef PARSING_HPP

#define PARSING_HPP

#include "headers.hpp"



bool 		isWhitespace(char c);
size_t		escape_white_space(_string my_string);
_string		erase_some_charc(_string my_string);
int 		parsing_config_file(_string file, _server_config &servers);
int			phase_to_req_res(_server_config &vec, std::vector<DataConf> &_vec_data);



#endif