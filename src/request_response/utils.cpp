#include "../../inc/utils.hpp"
// #include "../../inc/req_headers.hpp"
// #include "../../inc/req_res.hpp"
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <dirent.h>
#include <filesystem>

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

string removeNonPrintableChars(string str)
{
    size_t i = 0;
    while (str[i] != '\0' && (std::isspace(str[i]) || std::isprint(str[i]) == false))
	{
        i++;
    }
    return str.substr(i);
}

void print(string s)
{
    size_t i = 0;
    while (i < s.size())
    {
        if (s[i] == '\n')
		{
            std::cout << "\\n";
    		std::cout << std::endl;
		}
        else if (s[i] == '\r')
            std::cout << "\\r";
        else
            std::cout << s[i];
        // cout << s[i];
        i++;
    }
}

bool valid_http(string http_ver)
{
	if (_to_lower(http_ver) == "http/1.1" || _to_lower(http_ver) == "http/1.0")
		return true;
	
	return false;
}

void fill_query(string full_path, HTTP_request &request)
{
	size_t pos = full_path.find("?");
	if (pos == string::npos)
		return;
	request.query_string = full_path.substr(pos + 1);
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
		// std::cout << "========= " << i << '\n';
		if (dir_content[i] == "." || dir_content[i] == "..")
			continue;
		html += "<a href=\"" + file.file_path.substr(file.location.__root.length()) + "/" + dir_content[i] + "\">" + dir_content[i] + "</a>" + "<br>";
	}
	html += "</pre><hr></body></html>";
	// MIME type
	// std::cout << "body::" << response.body << std::endl;
	response.body += html;
	response.content_type = "text/html";
	response.content_length = std::to_string(html.length());
}

void fill_content_type(HTTP_request &req, string &content_type)
{
	std::stringstream ss(content_type);
	string line;
	size_t pos;

	// std::cout << "::" << content_type << std::endl;
	std::getline(ss, req.content_type.first, ';');
	req.content_type.first = removeNonPrintableChars(req.content_type.first);
	// std::cout << "+++content_type:" << req.content_type.first << "\n";
	std::getline(ss, line, '\r');
	line = removeNonPrintableChars(line);
	// std::cout << "+++line:" << line << "\n";
	pos = line.find("boundary=");
	if (pos != line.npos)
	{
		if (pos != 0)
		{
			// err with request 
			return;
		}
		// std::cout << "+++subline:" << line.substr(pos + 9) << "\n";
		req.content_type.second = line.substr(pos + 9);
	}
}

bool check_for_end_boundary(string &body, string &boundary)
{
	// print("body:" + body);
	// if (body.find("--" + boundary) != string::npos)
	// print(boundary);
	// print(body.substr(body.size() - 200));
	// if(body.find("--" + boundary + "--") == (body.size() - (boundary.size() + 6)))
	// 	std::cout << "true\n";
	// print(body + body.find("--" + boundary + "--"));
	// print(body.substr(body.size() - (boundary.size() + 6)));
	if (body.find("--" + boundary) != string::npos && (body.find("--" + boundary + "--") == (body.size() - (boundary.size() + 6))) )
		return true;
	return false;
}

bool find_boundary(std::stringstream &body_stream, string &boundary)
{
	string line;

	std::getline(body_stream, line, '\n');
	if ("--" + boundary + "\r" == line)
	{
		return true;
	}
	return false;
}

bool handle_content_disposition(std::stringstream &body_stream, form_part &part, string &boundary)
{
	string line;
	string trash;
	std::stringstream _stream;
	size_t pos;
	bool ret = false;

	// std::cout << body_stream.str() << "\n";
	// print(body_stream.str());
	// std::cout << std::getline(body_stream, line, '\n') << "\n";
	// std::cout << "==========\n";
	// std::ofstream ofs("test.txt");
	// print(body_stream.str());
	// ofs << body_stream.str() ;
	while ((std::getline(body_stream, line, '\r') && std::getline(body_stream, trash, '\n'))and line != string(""))
	{
		// std::cout << "entery:";
		if (trash != string(""))
			std::cout << "the trash is not empty\n";
		// std::cout << "================\n";
		// print("line:" + line + "++");
		// std::cout << "line:" << line << "\n";
		// print(trash);
		_stream.str(line);
		// getline(_stream, line, '\r');
		// _stream.str(line);
		// std::cout << "line:" << line << "\n";

		if (line == "--" + boundary)
		{
			std::cout << "function was broken here 1\n";
			break;
		}
		std::getline(_stream, line, ':');
		// std::cout << "line1:";
		// print(line);
		// break;
		if (_to_lower(line) == "content-disposition")
		{
			// print("content-disposition: " + line);
			// std::cout << "is content-disposition\n";
			ret = true;
			std::getline(_stream, line, ';');
			// std::cout << "line:" << line << "\n";
			if (line.find("form-data") == line.npos)
			{
				std::cout << "form-data not found\n";
				return false;
			}
			while (1)
			{
				std::getline(_stream, line, ';');
				// std::cout << "line2:" << line << "\n";
				if ((pos= line.find("filename=")) != line.npos)
				{
					part.filename = line.substr(pos + 10, line.rfind("\"") - 11);
					// cout << "0000" << part.filename << "0000" << std::endl;
				}
				else if ((pos = line.find("name=")) != line.npos)
				{

					part.name = line.substr(pos + 6, line.rfind("\"") - 7);
				}
				if (_stream.eof())
				{
					std::cout << "eof\n";
					// std::cout << "function was broken here 2\n";
					break;
				}
			}
			// break;
		}
		// // else if(_to_lower(line) == "Content-Type")
		// // {
		// // 	std::getline(_stream, line, '\r');
		// // 	part.content_type = line;
		// }
		if (body_stream.eof())
		{
			std::cout << "eof\n";
			// std::cout << "function was broken here 3\n";
			break;
		}
	}
	// std::cout << "----";
	// print(line);
	// std::cout << std::endl;
	if (line != "")
	{
		std::cout << "line is not \\r\n";
		return false;
	}
	return ret;
}

bool read_part(std::stringstream &body_stream, string &b, form_part &part)
{
	// add a static string to store the rest of the line;
	 
	string p;
	string boundary = "--" + b;
	// char c;
	size_t pos;
	string line;
	size_t max_size = (MAX_READ_SIZE / 10);
	// size_t current_pos;
	std::__1::streampos current_pos;

	char *buff;

	buff = new char[max_size];
	std::cout << "got here100\n";
	// std::ofstream file("tmp.txt", std::ios::in | std::ios::out | std::ios::trunc);
	// file.write(body_stream.str().substr(body_stream.str().size() - 500).c_str() , body_stream.str().size());
	while (1)// check also for max_read_size
	{
		// cout << "got here2\n";
		// print(p);
		// body_stream.get(c);
		// p.append(1, c);
		body_stream.read(buff, MAX_READ_SIZE / 10);
		p.append(buff, body_stream.gcount());
		// std::cout << "P: " << p.size() << "\n";
		pos = p.find(boundary + CRLF);
		// std::cout << "--" << pos << "=" << p.npos << "\n";
		// cout << p << std::endl;
		if ((pos != p.npos))
		{
			std::cout << "not end boundary found\n";
			std::cout << "current_pos: " << body_stream.tellg() << "\n";
			current_pos = (body_stream.tellg() - std::__1::streampos(p.size() - (pos + (boundary.size() + 2))));
			std::cout << "current_pos: " << current_pos << "\n";
			p.erase(pos);
			// pos = p.size() - (boundary.size() + 2);
			// body_stream.seekg(current_pos - std::__1::streampos(pos));
			body_stream.seekg(current_pos);
			// std::cout << line << std::endl;
			part.content.swap(p);
			return (true);
		}
		pos = p.find(boundary + "--" + CRLF);
		if ((pos != p.npos))
		{
			std::cout << "end boundary found\n";
			current_pos = (body_stream.tellg() - std::__1::streampos(p.size() - (pos + (boundary.size() + 4))));
			// current_pos = (body_stream.tellg() - std::__1::streampos(p.size() - (boundary.size() + 4)));
			p.erase(pos);

			// p.erase(pos);
			// pos = p.size() - (boundary.size() + 2);
			// body_stream.seekg(current_pos - std::__1::streampos(pos));
			body_stream.seekg(current_pos);

			// pos = p.size() - (boundary.size() + 4);
			// // pos = pos - (boundary.size() + 4);
			// current_pos = body_stream.tellg();
			// body_stream.seekg(current_pos - std::__1::streampos(pos));
			// body_stream.seekg(current_pos - pos);
			part.content.swap(p);
			return (false);
		}
		if (body_stream.eof())
		{
			std::cout << "function was broken here 4\n";
			// part.content.swap(p);
			return false;
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
			std::cout << "handle_content_disposition failed\n";
			part.clear();
			return false;
		}
		std::cout << "got to read_part\n";
		if (read_part(body_stream, boundary, part) == false)
		{
			std::cout << "part was pushed\n";
			// std::cout << "part.content: " << part.content << "\n";
			parts.push_back(part);
			break;
		}
		std::cout << "part was readed\n";
		parts.push_back(part);
		part = form_part();
		sleep(1);
	}
	return (true);
}

size_t write_to_file(string &file_path, string &content)
{
	std::ofstream file(file_path, std::ios::out | std::ios::trunc);

	if (file.is_open())
	{
		std::cout << content.size() << std::endl;
		// std::cout << content.substr(0, 300) << std::endl;
		file.write(content.c_str(), content.size());
		// file << content;
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
	std::cout << "------------->updating_file\n" << std::endl;
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
	std::cout << "------------->creating_file\n" << std::endl;
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

	std::cout << "----handeling parts----\n";
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
				out_file.close();
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
				// std::cout << "tmp_file:" << tmp_file << std::endl;
				// std::cout << "name:" << it->name << std::endl;
				out_file << it->name << " : " << it->content << '\n';
				it->success = DATA_SUCCESS;
				it->comment = it->name + " was added to " + tmp_file;
				out_file.close();
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


// deletion 
bool delete_file(string file_path, HTTP_response &response)
{
	if (std::remove(file_path.c_str()) != 0)
	{
		response.set_status(500, "Internal Server Error");
		return false;
	}
	else
	{
		response.set_status(204, "No Content");
		// response.content_type = file.content_type;
		// response.location = file.requested_path;
	}
	return true;
}

bool delete_internal_files(file_info &file, HTTP_response &response)
{
	vector<std::string> dir_content = read_dir(file.file_path);
	for (size_t i = 0; i < dir_content.size(); i++)
	{
		if (delete_file(file.file_path + dir_content[i], response) == false)
			return false;
	}
	return true;
}

void delete_dir(file_info file, HTTP_response &response)
{
	if (delete_internal_files(file, response) == false)
	{
		return;
	}

	if (rmdir(file.file_path.c_str()) != 0)
	{
		std::cout << "reasion : " << std::strerror(errno) << std::endl;
		response.set_status(500, "Internal Server Error");
	}
	else
	{
		response.set_status(204, "No Content");
	}
}

string trim_white_spaces(string str)
{
	size_t start = 0;
	size_t end = str.length() - 1;

	while (str[start] == ' ' || str[start] == '\t' || str[start] == '\r')
		start++;
	while (str[end] == ' ' || str[end] == '\t' || str[end] == '\r')
		end--;
	return str.substr(start, end - start + 1);
}
















