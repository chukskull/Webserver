#include "../../inc/Library.hpp"

    file_info servers_library::get_requested_file(string path, size_t s_index)
    {
        file_info info;
        ReqLoc loc;

        for (std::vector<ReqLoc>::iterator it = _servers[s_index].__locations.begin(); it != _servers[s_index].__locations.end(); it++)
        {

			// std::cout << "! get lucky\n";
            if (path.find(it->__path) != string::npos) // possibly check that the npos is zero
            {
                if (loc.__path.length() < it->__path.length())
					loc = *it;

            }
        }
		if (loc.is_set())
		{
			info.location = loc;
			info._allowMeth = loc._AllowMeth;
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

	void servers_library::set(vector<DataConf> &srvrs)
	{
		_servers = srvrs;
	}