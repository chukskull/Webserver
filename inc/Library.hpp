// #include "typedefs.hpp"
#pragma once
#include "req_res.hpp"
struct HTTP_request;

struct file_info
{
    string requested_path;
    string file_path;
    string content_type;

    bool is_dir;
    bool is_file;
    bool is_autoindex;
    bool is_redirect;
    bool file_exists;
    bool is_readable;
    bool is_writable;
	MethAllow _allowMeth;
    // bool is_allowed;

    ReqLoc location;

    file_info() : is_dir(false), is_file(false),is_autoindex(false), is_redirect(false), file_exists(false)
	{
		_allowMeth.resize(3, false);
	}
    // file_info &operator=(file_info &other)
    // {
    //     std::cout << "++++\n";
    //     this->_allowMeth = other._allowMeth;
    //     return (*this);
    // }
};

class servers_library 
{
    private:
    vector<DataConf> _servers;
    string _path;
    // map<short, string> status;
    map<short, string> status;
    map<short, string> html_error_pages;
    bool matched_host(DataConf &to_check, size_t index);
    bool matched_port(DataConf &to_check, Mesage &msg);
    // void fill_matched_servers();
    void fill_matched_servers(Mesage &msg, std::vector<DataConf> &m_servers);
    DataConf &find_best_server_match(HTTP_request &request_info, std::vector<DataConf> &m_servers);
	// static std::ofstream fake_db;

public:
    // servers_library() {fake_db.open("fake_db.txt", std::ios::app | std::ios::out); }
    servers_library() {}
    servers_library(vector<DataConf> servers);

    DataConf get_server_index(HTTP_request &request_info, Mesage &msg);
    // file_info get_requested_file(string path, size_t s_index);
    // file_info get_requested_file(HTTP_request &request_info, Mesage &msg);
    file_info get_requested_file(HTTP_request &request_info, DataConf &_server);
    // file_info get_requested_file(string path, size_t s_index, short &method);
    void set(vector<DataConf> &servers);
    string get_error_page(short status_code, string status_text);
    void set_error_pages(const std::map<int, std::string>& error_pages);
    // void set_error_pages(const std::map<short, std::string>& error_pages) ;
    string generate_error_page(int status_code, string status_message);
    void create_status_map();
    string get_status_text(short status_code);
};


// for GET requests
			//     // if it does, check if it is a file or a directory
			//     // if it is a file, check if it is allowed
			//     // if it is a directory, check if autoindex is allowed
			//     // if it is not allowed, return 403
			//     // if it is allowed, return 200
