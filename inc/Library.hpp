// #include "typedefs.hpp"
#pragma once
#include "req_res.hpp"

struct file_info
{
    string requested_path;
    string file_path;
    string content_type;

    bool file_exists;
    bool is_dir;
    bool is_file;
	MethAllow _allowMeth;
    // bool is_allowed;
    bool is_autoindex;
    bool is_redirect;

    ReqLoc location;

    file_info() : is_dir(false), is_file(false),is_autoindex(false), is_redirect(false), file_exists(false)
	{
		_allowMeth.resize(3, false);
	}
};

class servers_library 
{
    vector<DataConf> _servers;
    string _path;

public:
    servers_library() {}
    servers_library(vector<DataConf> servers) : _servers(servers) {}

    file_info get_requested_file(string path, size_t s_index);

};


// for GET requests
			//     // if it does, check if it is a file or a directory
			//     // if it is a file, check if it is allowed
			//     // if it is a directory, check if autoindex is allowed
			//     // if it is not allowed, return 403
			//     // if it is allowed, return 200