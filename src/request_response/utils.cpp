#include "../../inc/utils.hpp"
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

bool valid_http(string http_ver)
{
	if (http_ver == "http/1.1" || http_ver == "http/1.0")
		return true;
	
	return false;
}

string _to_lower(string str)
{
	return str;
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

vector<std::string> read_dir(const std::string& dir_path) {
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
		filenames.emplace_back(entry->d_name);
	}

	closedir(dir);

	return filenames;
}

void generate_autoindex(file_info file, HTTP_response &response)
{
	// you may file.file_path to file.requested_path

	string html = "<html><head><title>Index of " + file.file_path + "</title></head><body><h1>Index of " + file.file_path + "</h1><hr><pre><a href=\"../\">../</a>";
	vector<std::string> dir_content = read_dir(file.file_path);
	for (int i = 0; i < dir_content.size(); i++)
	{
		html += "<a href=\"" + dir_content[i] + "\">" + dir_content[i] + "</a>";
	}
	html += "</pre><hr></body></html>";
	// MIME type
	response.body += html;
	response.content_type = "text/html";
}

void fill_content_type(HTTP_request &req, string &content_type)
{
	req.content_type.first = content_type.substr(0, content_type.find(';'));
	req.content_type.second = content_type.substr(content_type.find(';') + 1, content_type.size());
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

bool handle_content_disposition(std::stringstream &body_stream, form_part &part)
{
	string line;
	std::stringstream _stream;
	size_t pos;
	bool ret = false;

	while (std::getline(body_stream, line, '\n') and line != string("\r"))
	{
		_stream.str(line);

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
	}
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

deque<form_part>    get_parts(string &body, string &boundary)
{
	deque<form_part> parts;
	std::stringstream body_stream(body);
	string line;

	form_part part;

	if (find_boundary(body_stream, boundary) == false)
	{
		//do shit
		return parts;
	}
	while (1)
	{
		if (handle_content_disposition(body_stream, part) == false)
		{
			part.clear();
			continue;
		}
		if (read_part(body_stream, boundary, part) == false)
		{
			parts.push_back(part);
			break;
		}
		parts.push_back(part);
		part = form_part();
	}
}
