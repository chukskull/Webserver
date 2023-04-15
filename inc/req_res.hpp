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
	size_t content_length;
	string connection;
	string body;
	//construsctor for request empty strings and -1 in numbers
	HTTP_request() : method(-1), requested_file(""), host(""), content_type(std::make_pair("", "")), content_length(-1), connection(""), body("") {}
	// multimap<float, string> accept;
	// vector<string, int> accept;
};

struct HTTP_response
{
	size_t status_code;
	bool close_connection;
	string status_text;
	string content_type;
	string location;
	string content_length;
	string connection;
	string body;

	bool stop;

	void set_status(short code, string text)
	{
		status_code = code;
		status_text = text;
		if (status_code > 399)
			stop = true;
	}
	HTTP_response(): stop(false) {}
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

class request;