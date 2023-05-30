#include "req_res.hpp"
#include "req_headers.hpp"
#include "mesage.hpp"
// #include "../src/request_response/request.cpp"
// #include "Library.hpp"
servers_library lib;
void print_meth(MethAllow meth)
{
	for (MethAllow::iterator it = meth.begin(); it != meth.end(); it++)
	{
		std::cout << *it << " ";
	}
	std::cout << std::endl;
}

void print_locations(ReqLoc &vec)
{

	std::cout << "loc== ";
	std::cout << "file: " << vec.__file;
	std::cout << " path: " << vec.__path;
	std::cout << " root: " << vec.__root << std::endl;
	print_meth(vec._AllowMeth);
}

void print_file(file_info &file)
{
	std::cout << "file path : " << file.file_path << "\n";
	print_locations(file.location);
	print_meth(file._allowMeth);
}

class __GET
{

public:
	__GET() {}

	void handle(HTTP_request &request_info, HTTP_response &response, Mesage &msg)
	{
		file_info file;
		DataConf _server_ = lib.get_server_index(request_info, msg);
		file = lib.get_requested_file(request_info, _server_);
		lib.set_error_pages(_server_.__error);

		if (file._allowMeth[con_GET])
		{
			if (file.is_redirect)
			{
				response.set_status(301, "Moved Permanently");
				response.location = file.location.__redirect.second;
			}
			else if (file.file_exists)
			{
				if (file.is_readable == false)
					response.set_status(403, "Forbidden");
				else if (file.file_dir_readable == false)
					response.set_status(403, "Forbidden");
				else if (file.is_dir)
				{
					if (file.is_autoindex)
					{
						generate_autoindex(file, response);
						response.set_status(200, "OK");
					}
					else
						response.set_status(403, "Forbidden");
				}
				else if (file.is_file)
				{
					response.set_status(200, "OK");
					if (file.location._cgi && (file_extention(file.file_path) == file.location.__cgi_ext))
					{
						create_env_(request_info, _server_, file);
						try
						{
							request_info.env_c = new char*[request_info.env_v.size() + 1];
							env_v_to_c(request_info.env_c, request_info.env_v);
							_cgi_info cgi_info;
							cgi_info.cgi_name = file.file_path;
							cgi_info.lang_path = file.location.__cgi_path;
							cgi_info.cgi_ext = file.location.__cgi_ext ;
							cgi(cgi_info , request_info, response);

							free_env(request_info.env_c, request_info.env_v.size());
						}
						catch(...)
						{
							response.set_status(500, "Internal Server Error");
						}
					}
					else
					{
						response.content_type = file.content_type;
						read_file(file.file_path, response);
					}
				}
			}
			else
			{
				response.set_status(404, "File Not Found");
			}

		}
		else
			response.set_status(405, "Method Not Allowed");
	}
private:
	void read_file(string file_path, HTTP_response &response)
	{
		std::ifstream file(file_path);
		string file_content;

		if (file.fail())
			response.set_status(500, "internal server error");
		else
		{
			string buffer;
			char *buff;
			size_t max_size = MAX_READ_SIZE;

			buff = new char[max_size];
			buffer.resize(max_size);
			while (1)
			{
				file.read(buff , max_size);
				try
				{
					file_content.append(buff, file.gcount());
				}
				catch (...)
				{
					response.set_status(503, "Service Unavailable");
					break;
				}
				if (file.eof())
					break;
			}
			file.close();
			delete [] buff;
			response.content_length = std::to_string(file_content.length());
			response.body.swap(file_content);
		}
	}
};

class __POST
{
public:
	void handle(HTTP_request &request_info, HTTP_response &response, Mesage &msg)
	{
		file_info file;

		if (check_transfer(request_info, response) == false)
			return ;
		DataConf _server_ = lib.get_server_index(request_info, msg);
		file = lib.get_requested_file(request_info, _server_);
		lib.set_error_pages(_server_.__error);

		if (file._allowMeth[con_POST])
		{	
			if (file.is_redirect)
			{
                response.set_status(301, "Moved Permanently");
                response.location = file.location.__redirect.second;
			}
			else
			{
				if (file.file_exists)
				{
					if (file.location._cgi)
					{
						try
						{
							_cgi_info cgi_info;
							create_env_(request_info, _server_, file);
							request_info.env_c = new char*[request_info.env_v.size() + 1];
							env_v_to_c(request_info.env_c, request_info.env_v);
							print_env(request_info.env_c, request_info.env_v.size());

							if (file_extention(file.file_path) == file.location.__cgi_ext)
							{
								cgi_info.cgi_name = file.file_path;
								cgi_info.lang_path = file.location.__cgi_path;
								cgi_info.cgi_ext = file.location.__cgi_ext ;
								cgi(cgi_info , request_info, response);
							}
							else
							{
								response.set_status(403, "Forbidden extention for cgi");
							}
							free_env(request_info.env_c, request_info.env_v.size());
						}
						catch(...)
						{
							response.set_status(500, "Internal Server Error");
						}
					}
					else if (file.is_writable == false || file.file_dir_writable == false)
						response.set_status(403, "Forbidden");

					else if (request_info.content_type.first == "multipart/form-data")
					{
						deque<form_part> parts;
						if (check_for_end_boundary(request_info.body, request_info.content_type.second))
						{
							if(get_parts(request_info.body, request_info.content_type.second, parts))
							{
								handle_parts(file, parts, request_info, response);
								generat_response(parts, response);
							}
							else
								response.set_status(400, "Bad Request1");
						}
						else
							response.set_status(400, "Bad Request2");
					}
					else if (file.is_dir)
						response.set_status(400, "Bad Request path");
					else
						update_file(file, request_info, response);
				}
				else
				{
					if (file.file_dir_exists)
					{
						if (file.file_dir_writable == false)
							response.set_status(403, "Forbidden not writable");
						else
							creat_file(file, request_info, response);
					}
					else
						response.set_status(404, "File Not Found");
				}
			}
		}
		else
			response.set_status(405, "Method Not Allowed");
	}
};

class __DELETE
{
public:
	void handle(HTTP_request &request_info, HTTP_response &response, Mesage &msg)
	{
		file_info file;

		DataConf _server_ = lib.get_server_index(request_info, msg);
		file = lib.get_requested_file(request_info, _server_);
		lib.set_error_pages(_server_.__error);

		if (file._allowMeth[con_DELETE])
		{
			if (file.is_redirect)
			{
				response.set_status(301, "Moved Permanently");
                response.location = file.location.__redirect.second;
			}
			else
			{
				if (file.file_exists)
				{
					if (file.is_writable == false)
						response.set_status(403, "Forbidden not writable");
					else if (file.is_file)
					{
						if (file.file_dir_writable)
							delete_file(file.file_path, response);
						else
							response.set_status(403, "Forbidden not writable");
					}
					else if (file.is_dir)
						delete_dir(file, response);
					else
						generate_error(response, 800, "no idea on why i have this condition here");
				}
				else
				{
					response.set_status(404, "File Not Found");
				}
			}
		}
		else
		{
			response.set_status(405, "Method Not Allowed");
		}
	}

};

class handler
{
	private:
		__GET GET_;
		__POST POST_;
		__DELETE DELETE_;
	public:

		handler() {}
		void handle(Mesage &msg)
		{
			request req;

			if (req.parse(msg.message) == 0)
			{
				if (req.request_info.method == GET)
					GET_.handle(req.request_info, req.response, msg);
				else if (req.request_info.method == POST)
					POST_.handle(req.request_info, req.response, msg);
				else if (req.request_info.method == DELETE)
					DELETE_.handle(req.request_info, req.response, msg);
			}
			fill_response(req, msg.response);
		}

		void manage_server_errors(short status_code, string &response, string status_text = "")
		{
			string error_page;

			if (status_text == "")
				status_text = lib.get_status_text(status_code);
			error_page = lib.get_error_page(status_code, status_text);
			response.append("HTTP/1.1 ");
			response.append(std::to_string(status_code) + " ");
			response.append(status_text);
			response.append(CRLF);

			response += "Content-Type: Text/html" ; response.append(CRLF);
			response += "Content-length: " + std::to_string(error_page.size()); response.append(CRLF);
			response.append(CRLF);

			response.append(error_page);
		}

	private:
	void fill_response(request &req, string &res)
	{
		res.append("HTTP/1.1 ");
		res.append(std::to_string(req.response.status_code) + " ");
		res.append(req.response.status_text);
		res.append(CRLF);

		set_cookies(req.request_info, req.response);
		if (req.response.body.empty())
		{
			if (req.response.status_code < 400)
				req.response.body = lib.generate_success_page(req.response.status_code, req.response.status_text);
			else if (req.response.status_code >= 400)
				req.response.body = lib.get_error_page(req.response.status_code, req.response.status_text);
		}
		if (req.response.content_length != "")
		{ res.append("Contnet-Length: "); res.append(req.response.content_length); res.append(CRLF);}

		if (req.response.content_type != "")
		{ res.append("Content-Type: "); res.append(req.response.content_type); res.append(CRLF);}
		
		if (req.response.location != "")
		{ res.append("Location: "); res.append(req.response.location); res.append(CRLF);}

		if (req.response.cookies != "")
		{ res.append(req.response.cookies); res.append(CRLF);}
		res.append(CRLF);
		res.append(req.response.body);
	}
};






