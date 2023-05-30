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
    bool file_dir_exists;
    bool file_dir_readable;
    bool file_dir_writable;
	MethAllow _allowMeth;

    ReqLoc location;

    file_info() : is_dir(false), is_file(false),is_autoindex(false), is_redirect(false), file_exists(false), is_readable(false), is_writable(false)
	{
		_allowMeth.resize(3, false);
	}
};

class servers_library 
{
    private:
    vector<DataConf> _servers;
    string _path;
    map<short, string> status;
    map<short, string> html_error_pages;
    bool matched_host(DataConf &to_check, size_t index);
    bool matched_port(DataConf &to_check, Mesage &msg);
    void fill_matched_servers(Mesage &msg, std::vector<DataConf> &m_servers);
    DataConf &find_best_server_match(HTTP_request &request_info, std::vector<DataConf> &m_servers);

public:
    servers_library() {}
    servers_library(vector<DataConf> servers);

    DataConf get_server_index(HTTP_request &request_info, Mesage &msg);
    file_info get_requested_file(HTTP_request &request_info, DataConf &_server);
    void set(vector<DataConf> &servers);
    string get_error_page(short status_code, string status_text);
    void set_error_pages(const std::map<int, std::string>& error_pages);
    string generate_error_page(int status_code, string status_message);
    void create_status_map();
    string get_status_text(short status_code);
    std::string generate_success_page(short status_code, const std::string& status_text);
    string get_success_page(short status_code);
};
