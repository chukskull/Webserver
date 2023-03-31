#ifndef REQUESTCONFIG_HPP
#define REQUESTCONFIG_HPP

#include "headers.hpp"



typedef  std::vector<bool> MethAllow;

struct ReqLoc
{
    bool                        _autoindex;
    _string		                __path;
    _string                     __file;
    _string                     __root;
    MethAllow                   _AllowMeth;
    std::pair<bool, std::string> __redirect;    
};




struct DataConf
{
    size_t             __body_size;
    _string             __host;
    _string             __name;
    _string             __port;
    std::vector<ReqLoc> __locations;
};


#endif