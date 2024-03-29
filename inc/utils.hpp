#pragma once
#define MAX_READ_SIZE 100000
#include "typedefs.hpp"
#include "req_res.hpp"
// #include "typedefs.hpp"

bool file_exist(string file_path);
bool file_is_readable(string file_path);
bool file_is_writable(string file_path);
bool valid_http(string http_ver);
string _to_lower(string str);
bool is_dir(string path);
bool is_file(string path);

struct form_part;
struct file_info;
struct HTTP_request;
struct HTTP_response;
struct DataConf;


size_t write_to_file(string &file_path, string &content);
void update_file(file_info file, HTTP_request &request_info, HTTP_response &response);
void generate_error(HTTP_response &response, int status_code, string status_message);
void handle_parts(file_info file , deque<form_part> &parts, HTTP_request &request_info, HTTP_response &response);
string generat_response(deque<form_part> &parts, HTTP_response &response);


void creat_file(file_info file, HTTP_request &request_info, HTTP_response &response);

vector<std::string> read_dir(const std::string& dir_path);
bool read_part(std::stringstream &body_stream, string &b, form_part &part);
// bool get_parts(string &body, string &boundary, deque<form_part> parts);
bool get_parts(string &body, string &boundary, deque<form_part> &parts);

void fill_content_type(HTTP_request &req, string &content_type);

void generate_autoindex(file_info file, HTTP_response &response);

void fill_content_type(HTTP_request &req, string &content_type);
bool check_for_end_boundary(string &body, string &boundary);
bool find_boundary(std::stringstream &body_stream, string &boundary);
bool handle_content_disposition(std::stringstream &body_stream, form_part &part, string &boundary);


// void delete_file(file_info file, HTTP_response &response);
bool delete_file(string file_path, HTTP_response &response);
void delete_dir(file_info file, HTTP_response &response);

string get_exten(string file);

void print(string s);
string trim_white_spaces(string str);
void fill_query(string &full_path, HTTP_request &request);

string  file_extention(string file);
void	create_env_(HTTP_request &request_info, DataConf &_server_, file_info &file);
void    free_env(char **env, size_t size);
void	print_env(char **env, size_t size);
void    env_v_to_c(char **env, vector<string> &env_v);

void  percent_encoding(string &str);
bool check_transfer(HTTP_request &request_info,HTTP_response &response);
string get_dir(string file_path);
string	generate_other_file_name(string tmp_file);