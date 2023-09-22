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
	if (access(file_path.c_str(), F_OK) == 0)
		return (true);
	return (false);
	// std::ifstream file(file_path);
	// if (file.fail())
	// 	return (false);
	// else
	// {
	// 	file.close();
	// 	return (true);
	// }
}

bool file_is_readable(string file_path)
{
	if (access(file_path.c_str(), R_OK) == 0)
		return (true);
	return (false);
}

bool file_is_writable(string file_path)
{
	if (access(file_path.c_str(), W_OK) == 0)
		return (true);
	return (false);
}

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

void fill_query(string &full_path, HTTP_request &request)
{
	size_t pos = full_path.find("?");
	if (pos == string::npos)
		return;
	request.query_string = full_path.substr(pos + 1);
	request.requested_file = full_path.substr(0, pos);
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
		return filenames;

	struct dirent* entry;
	while ((entry = readdir(dir)))
	{
		if (entry->d_name[0] == '.')
			continue;
		filenames.push_back(entry->d_name);
	}
	closedir(dir);
	return filenames;
}

void generate_autoindex(file_info file, HTTP_response& response)
{
    string html = "<html><head><title>Index of " + file.file_path + "</title>";
    html += "<style>";
    html += "body { font-family: Arial, sans-serif; background-color: #f1f1f1; padding: 20px; }";
    html += ".container { max-width: 600px; margin: 0 auto; background-color: #fff; padding: 20px; border-radius: 5px; box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1); }";
    html += "h1 { color: #333; text-align: center; }";
    html += "ul { list-style-type: none; padding: 0; margin: 0; }";
    html += "li { margin-bottom: 10px; }";
    html += ".button { display: inline-block; background-color: #007bff; color: #fff; padding: 10px 20px; text-decoration: none; border-radius: 4px; }";
    html += ".button:hover { background-color: #0056b3; }";
    html += "</style>";
    html += "</head><body>";
    html += "<div class=\"container\">";
    html += "<h1>Index of " + file.file_path + "</h1><hr>";
    html += "<ul>";

    vector<string> dir_content = read_dir(file.file_path);
    for (size_t i = 0; i < dir_content.size(); i++)
    {
        if (dir_content[i] == "." || dir_content[i] == "..")
            continue;

        html += "<li><a href=\"" + file.location.__path;
        if (file.file_path.substr(file.location.__root.length()) != "")
            html += file.file_path.substr(file.location.__root.length()) + "/" + dir_content[i] + "\">" + dir_content[i] + "</a></li>";
        else
            html += "/" + dir_content[i] + "\">" + dir_content[i] + "</a></li>";
    }

    html += "</ul><hr>";
    html += "<a href=\"/\" class=\"button\">Go Back</a>";
    html += "</div></body></html>";

    response.body += html;
    response.content_type = "text/html";
    response.content_length = std::to_string(html.length());
}



void fill_content_type(HTTP_request &req, string &content_type)
{
	std::stringstream ss(content_type);
	string line;
	size_t pos;

	std::getline(ss, req.content_type.first, ';');
	req.content_type.first = removeNonPrintableChars(req.content_type.first);
	std::getline(ss, line, '\r');
	line = removeNonPrintableChars(line);
	pos = line.find("boundary=");
	if (pos != line.npos)
	{
		if (pos != 0)
			return;
		req.content_type.second = line.substr(pos + 9);
	}
}

bool check_for_end_boundary(string &body, string &boundary)
{
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

	while ((std::getline(body_stream, line, '\r') && std::getline(body_stream, trash, '\n'))and line != string(""))
	{
		if (trash != string(""))
			std::cout << "the trash is not empty\n";
		_stream.str(line);

		if (line == "--" + boundary)
			break;
		std::getline(_stream, line, ':');
		if (_to_lower(line) == "content-disposition")
		{
			ret = true;
			std::getline(_stream, line, ';');
			if (line.find("form-data") == line.npos)
				return false;
			while (1)
			{
				std::getline(_stream, line, ';');
				if ((pos= line.find("filename=")) != line.npos)
					part.filename = line.substr(pos + 10, line.rfind("\"") - 11);
				else if ((pos = line.find("name=")) != line.npos)
					part.name = line.substr(pos + 6, line.rfind("\"") - 7);
				if (_stream.eof())
					break;
			}
		}
		if (body_stream.eof())
			break;
	}
	if (line != "")
		return false;
	return ret;
}

bool read_part(std::stringstream &body_stream, string &b, form_part &part)
{
	string p;
	string boundary = "--" + b;
	size_t pos;
	string line;
	size_t max_size = (MAX_READ_SIZE / 10);
	std::__1::streampos current_pos;
	char *buff;

	buff = new char[max_size];
	while (1)
	{
		body_stream.read(buff, MAX_READ_SIZE / 10);
		p.append(buff, body_stream.gcount());
		pos = p.find(boundary + CRLF);
		if ((pos != p.npos))
		{
			current_pos = (body_stream.tellg() - std::__1::streampos(p.size() - (pos + (boundary.size() + 2))));
			p.erase(pos);
			body_stream.seekg(current_pos);
			part.content.swap(p);
			delete buff;
			return (true);
		}
		pos = p.find(boundary + "--" + CRLF);
		if ((pos != p.npos))
		{
			current_pos = (body_stream.tellg() - std::__1::streampos(p.size() - (pos + (boundary.size() + 4))));
			p.erase(pos);
			body_stream.seekg(current_pos);
			part.content.swap(p);
			delete buff;
			return (false);
		}
		if (body_stream.eof())
		{
			delete buff;
			return false;
		}
	}
	delete buff;
}

bool get_parts(string &body, string &boundary, deque<form_part> &parts)
{
	std::stringstream body_stream(body);
	string line;
	form_part part;

	if (find_boundary(body_stream, boundary) == false)
		return false;
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
		file.write(content.c_str(), content.size());
		file.close();
		return (200);
	}
	else
		return 500;
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
			tmp_file = generate_other_file_name(tmp_file);
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


bool delete_file(string file_path, HTTP_response &response)
{
	if (std::remove(file_path.c_str()) != 0)
	{
		response.set_status(500, "Internal Server Error");
		return false;
	}
	else
		response.set_status(204, "No Content");
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
		response.set_status(500, "Internal Server Error");
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

string file_extention(string file)
{
	size_t pos = file.find_last_of('.');
	if (pos == string::npos)
		return "";
	return file.substr(pos);
}


void add_to_env(string key, string val,HTTP_request &request_info)
{
	string tmp;

	tmp = key + "=" + val;
	tmp.append(1, '\0');
	request_info.env_v.push_back(tmp);
}

string get_method(short method)
{
	if (method == GET)
		return "GET";
	if (method == POST)
		return "POST";
	if (method == DELETE)
		return "DELETE";
	return "NOT SUPORTED";
}

void	create_env_(HTTP_request &request_info, DataConf &_server_, file_info &file)
{
	string tmp;
	add_to_env("SERVER_SOFTWARE", "ta7 server- v:9.6", request_info);
	add_to_env("SERVER_NAME", _server_.__host, request_info);
	add_to_env("GATEWAY_INTERFACE", "CGI/1.1", request_info);
	add_to_env("SERVER_PROTOCOL", "HTTP/1.1", request_info);
	add_to_env("REQUEST_METHOD", get_method(request_info.method), request_info);
	add_to_env("SCRIPT_NAME", file.file_path, request_info);
	if (request_info.method == GET)
		add_to_env("QUERY_STRING", request_info.query_string, request_info);
	else if (request_info.method == POST)
		add_to_env("QUERY_STRING", request_info.body.erase(request_info.body.size() - 1, 1), request_info);

	if (request_info.content_length != -1)
		add_to_env("CONTENT_LENGTH", std::to_string(request_info.content_length), request_info);
	else 
		add_to_env("CONTENT_LENGTH", std::to_string(0), request_info);
	if (request_info.content_type.first != "")
		add_to_env("CONTENT_TYPE", request_info.content_type.first, request_info);
}

void env_v_to_c(char **env, vector<string> &env_v)
{
	for (size_t i = 0; i < env_v.size(); i++)
	{
		env[i] = new char[env_v[i].size() + 1];
		std::strcpy(env[i], env_v[i].c_str());
	}
	env[env_v.size()] = NULL;
}

void free_env(char **env, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		delete[] env[i];
	}
	delete[] env;
}

void print_env(char **env, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		std::cout << "env[" <<i << "]" << env[i] << std::endl;
	}
}

void fill_percent_incoding(map<string, string> &PercentEncoding)
{
	PercentEncoding.insert(std::make_pair("%3A",":"));
    PercentEncoding.insert(std::make_pair("%2F","/"));
    PercentEncoding.insert(std::make_pair("%3F","?"));
    PercentEncoding.insert(std::make_pair("%23","#"));
    PercentEncoding.insert(std::make_pair("%5B","["));
    PercentEncoding.insert(std::make_pair("%5D","]"));
    PercentEncoding.insert(std::make_pair("%40","@"));
    PercentEncoding.insert(std::make_pair("%21","!"));
    PercentEncoding.insert(std::make_pair("%24","$"));
    PercentEncoding.insert(std::make_pair("%26","&"));
    PercentEncoding.insert(std::make_pair("%27","'"));
    PercentEncoding.insert(std::make_pair("%28","("));
    PercentEncoding.insert(std::make_pair("%29",")"));
    PercentEncoding.insert(std::make_pair("%2A","*"));
    PercentEncoding.insert(std::make_pair("%2B","+"));
    PercentEncoding.insert(std::make_pair("%2C",","));
    PercentEncoding.insert(std::make_pair("%3B",";"));
    PercentEncoding.insert(std::make_pair("%3D","="));
    PercentEncoding.insert(std::make_pair("%25","%"));
    PercentEncoding.insert(std::make_pair("%20"," "));
	PercentEncoding.insert(std::make_pair("%22","\""));
	PercentEncoding.insert(std::make_pair("%3C","<"));
	PercentEncoding.insert(std::make_pair("%3E",">"));
}

void  percent_encoding(string &str)
{
	map<string, string> PercentEncoding;
   	
	fill_percent_incoding(PercentEncoding);
	for (map<string, string>::iterator it = PercentEncoding.begin(); it != PercentEncoding.end(); it++)
	{
		size_t pos = 0;
		while ((pos = str.find(it->first, pos)) != string::npos)
		{
			str.replace(pos, it->first.length(), it->second);
			pos += it->second.length();
		}
	}
}

bool check_transfer(HTTP_request &request_info,HTTP_response &response)
{
	if ((request_info.content_length == -1 && request_info.content_encoding != "chunked") || \
		(request_info.content_length != -1 && request_info.content_encoding == "chunked"))
	{
		response.set_status(400, "Bad Request");
		return false;
	}
	return true;
}

string get_dir(string file_path)
{
	size_t pos = file_path.find_last_of('/');
	if (pos == string::npos)
		return "";
	return file_path.substr(0, pos + 1);
}

string	generate_other_file_name(string tmp_file)
{
	size_t pos = tmp_file.find_last_of('.');
	string extention = tmp_file.substr(pos);
	string file_name = tmp_file.substr(0, pos);
	string new_file_name;
	int i = 0;

	while (1)
	{
		new_file_name = file_name + std::to_string(i) + extention;
		if (access(new_file_name.c_str(), F_OK) == -1)
			break;
		i++;
	}
	return new_file_name;
}
