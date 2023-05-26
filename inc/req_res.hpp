#pragma once

#include <fstream>
#include "typedefs.hpp"
#include "utils.hpp"
#include "MIMEs.hpp"
#include "headers.hpp"
#include "Library.hpp"
// #include "RequestConfig.hpp"
// servers_library lib;
#define CRLF "\r\n"

struct ReqLoc;
enum
{
	//methodes
	GET = 1,
	POST,
	DELETE,
	//athontication
	ALLOWED,
	NOT_ALLOWED,
	// 
	DROP,
	MOVE,
	CLOSE_CONNECTION,
	KEEP_ALIVE
};
enum 
{
	METHOD = 0,
	SOURCE,
	PROTOCOL,

	FILE_SUCCESS,
	DATA_SUCCESS,
	FAIL
};

struct HTTP_request
{
	short method;
	string requested_file;
	string host;
	std::pair<string, string> content_type;
	// string content_type;
	long long content_length;
	string connection;
	string body;
	string query_string;
	string cookies;
	vector<string> env_v;
	char ** env_c;
	//construsctor for request empty strings and -1 in numbers
	HTTP_request() : method(-1), requested_file(""), host(""), content_type(std::make_pair("", "")), content_length(-1), connection(""), body("") {}
	// multimap<float, string> accept;
	// vector<string, int> accept;
};

struct HTTP_response
{
	size_t status_code;
	string status_text;

	string content_length;
	string content_type;
	string location;
	string connection;
	bool close_connection;

	string body;
	string cookies;

	bool stop;

	void set_status(short code, string text)
	{
		status_code = code;
		status_text = text;
		if (status_code > 399)
			stop = true;
	}
	HTTP_response(): status_code(180), stop(false){}
	void set_stop(bool b){stop = b;}
	//string content_encoding; :: possibly
};

struct form_part
{
	string name;
	string filename;
	string content_type;
	string content;
	short success;
	string comment;
	// set all values to empty string
	form_part() : name(""), filename(""), content_type(""), content(""), success(FAIL) {}
	void clear()
	{
		name = "";
		filename = "";
		content_type = "";
		content = "";
		success = FAIL;
	}
};

class request
{
	vector<string> start_line;
	vector<pair<string, string> > headers;
	string body;

public:

	HTTP_request request_info;
	HTTP_response response;

	request();
	request(string req);
	void request_checkpoint();
	void parse(string req);

private:

	short method_num();
	int fill_req();
	void validate_start_line();
	
public:
	void print_req();
	void print_req(std::string req);
	void print(string s);
	
};

struct _cgi_info
{
	string lang_path;
	string cgi_name;
	string cgi_ext;
	bool __cgi_on;
};
// 	string cgi_extention;
// };
//-----------------///CGI///----------------//
int cgi(_cgi_info cgi_info , HTTP_request &request_info, HTTP_response &response);


//-----------------///cookies///----------------//
void set_cookies(HTTP_request &request_info, HTTP_response &response);
