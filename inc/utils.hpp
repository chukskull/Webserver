#define MAX_READ_SIZE 10000
#include "req_res.hpp"
#include "typedefs.hpp"

bool file_exist(string file_path);
bool valid_http(string http_ver);
string _to_lower(string str);
bool is_dir(string path);
bool is_file(string path);

bool get_parts(string &body, string &boundary, deque<form_part> parts);