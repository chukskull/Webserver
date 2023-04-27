#include "../../inc/utils.hpp"
// #include "../../inc/req_headers.hpp"
// #include "../../inc/req_res.hpp"
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <dirent.h>

bool file_exist(string file_path)
{
	std::ifstream file(file_path);
	if (file.fail())
		return (false);
	else
	{
		file.close();
		return (true);
	}
}

// std::string str_toupper(char *str)
// {
//     std::string new_str = str;
//     int len = new_str.size();
//     for(int j = 0; j < len; j++)
//     {
//         new_str[j] = std::toupper(new_str[j]);
//     }
//     return (new_str);
// }

string _to_lower(string str)
{
	for (string::iterator it = str.begin(); it != str.end(); ++it)
	{
		*it = std::tolower(*it);
	}
	return str;
}

bool valid_http(string http_ver)
{
	if (_to_lower(http_ver) == "http/1.1" || _to_lower(http_ver) == "http/1.0")
		return true;
	
	return false;
}

bool is_dir(string path)
{
	struct stat statbuf;
	if (stat(path.c_str(), &statbuf) != 0)
		return false;
	return S_ISDIR(statbuf.st_mode);
}

bool is_file(string path)
{
	struct stat statbuf;
	if (stat(path.c_str(), &statbuf) != 0)
		return false;
	return S_ISREG(statbuf.st_mode);
}

vector<std::string> read_dir(const std::string& dir_path)
{
	std::vector<std::string> filenames;

	DIR* dir = opendir(dir_path.c_str());
	if (!dir)
	{
		// handle error here
		return filenames;
	}

	struct dirent* entry;
	while ((entry = readdir(dir)))
	{
		if (entry->d_name[0] == '.')
		{
			continue;
		}
		filenames.push_back(entry->d_name);
	}

	closedir(dir);

	return filenames;
}

void generate_autoindex(file_info file, HTTP_response &response)
{
	// you may file.file_path to file.requested_path

	string html = "<html><head><title>Index of " + file.file_path + "</title></head><body><h1>Index of " + file.file_path + "</h1><hr><pre><a href=\"../\">../</a>";
	vector<std::string> dir_content = read_dir(file.file_path);
	for (size_t i = 0; i < dir_content.size(); i++)
	{
		// std::cout << "++++++>" << file.file_path << "\n";
		// std::cout << "------>" << dir_content[i] << "\n";
		html += "<a href=\"" + file.file_path.substr(file.location.__root.length()) + "/" + dir_content[i] + "\">" + dir_content[i] + "</a>" + "<br>";
	}
	html += "</pre><hr></body></html>";
	// MIME type
	response.body += html;
	response.content_type = "text/html";
}

void fill_content_type(HTTP_request &req, string &content_type)
{
	std::stringstream ss(content_type);
	string line;
	size_t pos;

	std::getline(ss, req.content_type.first, ';');
	std::getline(ss, line, '\r');
	pos = line.find("boundary=");
	if (pos != line.npos)
	{
		if (pos != 0)
		{
			// err with request 
			return;
		}
		req.content_type.second = line.substr(line.size() + 9);
	}
}

bool check_for_end_boundary(string &body, string &boundary)
{
	if (body.find(boundary) != string::npos && body.find(boundary + "--") == body.size() - (boundary.size() + 2))
		return true;
	return false;
}

bool find_boundary(std::stringstream &body_stream, string &boundary)
{
	string line;

	std::getline(body_stream, line);
	if (boundary + '\r' == line)
		return true;
	return false;
}

bool handle_content_disposition(std::stringstream &body_stream, form_part &part, string &boundary)
{
	string line;
	std::stringstream _stream;
	size_t pos;
	bool ret = false;

	while (std::getline(body_stream, line, '\n') and line != string("\r"))
	{
		_stream.str(line);

		if (line == "--" + boundary + '\r')
			break;

		std::getline(_stream, line, ':');
		if (_to_lower(line) == "Content-Disposition")
		{
			ret = true;
			std::getline(_stream, line, ';');
			if (line.find("form-data") == line.npos)
			{
				return false;
			}
			while (1)
			{
				std::getline(_stream, line, ';');
				if ((pos= line.find("filename=")) != line.npos)
				{
					part.filename = line.substr(pos + 10, line.rfind("\"") - 11);
					cout << "0000" << part.filename << "0000" << std::endl;
				}
				else if ((pos = line.find("name=")) != line.npos)
				{

					part.name = line.substr(pos + 6, line.rfind("\"") - 7);
				}
				if (_stream.eof())
					break;
			}
		}
		else if(_to_lower(line) == "Content-Type")
		{
			std::getline(_stream, line, '\r');
			part.content_type = line;
		}
		if (body_stream.eof())
			break;
	}
	if (line != "\r")
		return false;
	return ret;
}

bool read_part(std::stringstream &body_stream, string &b, form_part &part)
{
	string p;
	string boundary = "--" + b;
	char c;
	size_t pos;

	while (1)// check also for max_read_size
	{
		// cout << "got here2\n";
		body_stream.get(c);
		p.append(1, c);
		pos = p.rfind(boundary + "\r\n");
		// cout << p << std::endl;
		if ((pos != p.npos))
		{
			p.erase(pos);
			part.content.swap(p);
			return (true);
		}
		pos = p.rfind(boundary + "--\r\n");
		if ((pos != p.npos))
		{
			p.erase(pos);
			part.content.swap(p);
			return (false);
		}
	}
}

bool get_parts(string &body, string &boundary, deque<form_part> &parts)
{
	std::stringstream body_stream(body);
	string line;

	form_part part;

	if (find_boundary(body_stream, boundary) == false)
	{
		return false;
	}
	while (1)
	{
		if (handle_content_disposition(body_stream, part, boundary) == false)
		{
			part.clear();
			return false;
		}
		if (read_part(body_stream, boundary, part) == false)
		{
			parts.push_back(part);
			break;
		}
		parts.push_back(part);
		part = form_part();
	}
	return (true);
}


size_t write_to_file(string &file_path, string &content)
{
	std::ofstream file(file_path, std::ios::out | std::ios::trunc);

	if (file.is_open())
	{
		file << content;
		file.close();
		return (200);
	}
	else
	{
		return 500;
		// response.status_code = 500;
		// response.status_message = "Internal Server Error";
	}
}

void update_file(file_info file, HTTP_request &request_info, HTTP_response &response)
{
	if (write_to_file(file.file_path, request_info.body) >= 500)
	{
		response.set_status(500, "Internal Server Error");
		return;
	}
	else
	{
		response.set_status(200, "OK");
		response.content_type = file.content_type;
		response.location = file.requested_path;
	}
}

void creat_file(file_info file, HTTP_request &request_info, HTTP_response &response)
{
	if (write_to_file(file.file_path, request_info.body) >= 500)
	{
		response.set_status(500, "Internal Server Error");
		return;
	}
	else
	{
		response.set_status(201, "Created");
		response.content_type = file.content_type;
		response.location = file.requested_path;
	}
}

void generate_error(HTTP_response &response, int status_code, string status_message)
{
	response.set_status(status_code, status_message);
	response.content_type = "text/html";
	response.body = "<html><head><title>" + std::to_string(status_code) + " " + status_message + " it is a directory " + "</title></head><body><h1>" + std::to_string(status_code) + " " + status_message + "</h1></body></html>";
}
string get_exten(string file)
{
	MIME_types MIME;
	string s_type;

	s_type = MIME.get_extention(file);
    return (MIME.get_type(s_type));
}

void handle_parts(file_info file , deque<form_part> &parts, HTTP_request &request_info, HTTP_response &response)
{
	std::ofstream out_file;

	(void)request_info;
	(void)response;

	for (deque<form_part>::iterator it = parts.begin(); it != parts.end(); it++)
	{
		string tmp_file;
		MIME_types MIME;

		if (it->filename != "")
		{
			tmp_file = file.file_path + it->filename;
			if (!MIME.is_MIME_type(MIME.get_extention(tmp_file)))
			{
				if (it->content_type != "")
					tmp_file = tmp_file + MIME.get_MIME_extention(it->content_type);
			}
			out_file.open(tmp_file, std::ios::out | std::ios::trunc);
			if (out_file.is_open())
			{
				out_file << it->content;
				it->success = FILE_SUCCESS;
			}
			else
			{
				it->comment = "unable to open file : " + tmp_file ;
			}
		}
		else if (it->name != "")
		{
			tmp_file = file.file_path + "fake_db";
			out_file.open(tmp_file, std::ios::out | std::ios::app);
			if (out_file.is_open())
			{
				out_file << it->name << " : " << it->content << '\n';
				it->success = DATA_SUCCESS;
				it->comment = it->name + " was added to " + tmp_file;
			}
			else
			{
				it->comment = tmp_file + " is unavilable";
			}
		}
	}
}

string generat_response(deque<form_part> &parts, HTTP_response &response)
{
	string body;

	response.set_status(500, "Internal Server Error");
	for (deque<form_part>::iterator it = parts.begin(); it != parts.end(); it++)
	{
		if (it->success != FAIL)
			response.set_status(207, "Multi-Status");
		body += it->comment + '\n';
	}
	return body;
}






















