#include "typedefs.hpp"
#include "Req_res.hpp"

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

    file_info get_requested_file(string path, size_t s_index)
    {
        file_info info;
        ReqLoc loc;

        for (std::vector<ReqLoc>::iterator it = _servers[s_index].__locations.begin(); it != _servers[s_index].__locations.end(); it++)
        {
            if (path.find(it->__path) != string::npos) // possibly check that the npos is zero
            {
                if (loc.__path.length() < it->__path.length())
					loc = *it;
            }
        }
		if (loc.is_set())
		{
			info.location = loc;
			info.requested_path = path;
            info.file_path = loc.__root + path.substr(loc.__path.length());
			// info.type = path.substr(path.rfind('.') + 1);

			if (loc.__redirect.first)
			{
				info.is_redirect = true;
			}
			else
			{
				// check if the file exists
				if (file_exist(info.file_path))
				{
					info.file_exists = true;
					info._allowMeth = loc._AllowMeth;
					// check if it is a directory
					if (is_dir(info.file_path))
					{
						info.is_dir = true;
						// check if autoindex is allowed
						if (loc._autoindex)
						{
							info.is_autoindex = true;
						}
					}
					else if (is_file(info.file_path))
					{
						info.is_file = true;
					}
				}
			}
		}
		return info;
    }
};

servers_library lib;

// for GET requests
			//     // if it does, check if it is a file or a directory
			//     // if it is a file, check if it is allowed
			//     // if it is a directory, check if autoindex is allowed
			//     // if it is not allowed, return 403
			//     // if it is allowed, return 200
