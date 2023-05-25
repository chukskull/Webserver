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
	// std::cout << "locs== \n";
	// for (std::vector<ReqLoc>::iterator it = vec.begin(); it != vec.end(); it++)
	// {
	std::cout << "loc== ";
	std::cout << "file: " << vec.__file;
	std::cout << " path: " << vec.__path;
	std::cout << " root: " << vec.__root << std::endl;
	print_meth(vec._AllowMeth);
	// }
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
		// file = lib.get_requested_file(request_info.requested_file, msg._connections.second.first, request_info.method);
		// std::cout << "===========>:" << request_info.cookies << std::endl;
		DataConf _server_ = lib.get_server_index(request_info, msg);
		std::cout << "::" <<  _server_.__host << std::endl;
		// std::cout << "::" <<  _server_.__locations[0].__path << std::endl;
		std::cout << "requested file:" << request_info.requested_file << std::endl;
		file = lib.get_requested_file(request_info, _server_);
		std::cout << "------::file:" << file.file_path << std::endl;

		print_file(file);
		// std::cout << "requested_path" << file.requested_path << std::endl;
		if (request_info.connection != "")
		{
			if (request_info.connection == "close")
			{
				response.connection = CLOSE_CONNECTION;
				response.connection = "close";
			}
			else
			{
				response.connection = KEEP_ALIVE;
				response.connection = "Keep-alive";
			}
		}
		if (file._allowMeth[con_GET])
		{
			if (file.is_redirect)
			{
				// std::cout << "::" << file.is_redirect << std::endl;
				response.set_status(301, "Moved Permanently");
				response.location = file.location.__redirect.second;
			}
			else if (file.file_exists)
			{
				std::cout << "::" << file.location.__file << std::endl;
				if (file.is_dir)
				{
					if (file.is_autoindex)
					{
						std::cout << "got into autoindex\n";
						generate_autoindex(file, response);
						response.set_status(200, "OK");
						// serve autoindex
						// create the html page
					}

					else
					{
						response.set_status(403, "Forbidden");
					}
				}
				else if (file.is_file)
				{
					response.set_status(200, "OK");
					// _cgi_info cgi_info;
					if (file.location._cgi && (file_extention(file.file_path) == file.location.__cgi_ext))
					{
						create_env_(request_info, _server_, file);

						request_info.env_c = new char*[request_info.env_v.size() + 1];
						env_v_to_c(request_info.env_c, request_info.env_v);
						// print_env(request_info.env_c, request_info.env_v.size());

							std::cout << "run cgi\n";
							_cgi_info cgi_info;
							cgi_info.cgi_name = file.file_path;
							cgi_info.lang_path = file.location.__cgi_path;
							cgi_info.cgi_ext = file.location.__cgi_ext ;
							cgi(cgi_info , request_info, response);

							// free_env(request_info.env_c, request_info.env_v.size());
						// }
						// else
						// {
							// response.set_status(403, "Forbidden extention for cgi");
						// }
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
				response.set_status(404, "File Not Found 444");
			}

		}
		else
		{
			response.set_status(405, "Method Not Allowed1");
			// change the content type to maybe html
		}
	}
private:
	void read_file(string file_path, HTTP_response &response)
	{
		std::ifstream file(file_path);
		string file_content;

		if (file.fail())
		{
			response.set_status(500, "file couldn't be opend");
			response.body += "The server couldn't open the file try agin later.";
			// return ;
		}
		else
		{
			string buffer;
			char *buff;
			size_t max_size = MAX_READ_SIZE;

			// max_size = set_max_size(host) ;

			buff = new char[max_size];
			// file.seekg(0, std::ios::end);
   			// size_t file_size = file.tellg();
			// file.seekg(0, std::ios::beg);
			// file_content.resize(file_size);
			// file.read(&file_content[0], file_size);

			buffer.resize(max_size);
			while (1)
			{
				file.read(buff , max_size);
				try
				{
					// std::cout << "gcount: " << file.gcount() << std::endl;
					file_content.append(buff, file.gcount());
					// std::cout << "------>" << file_content.length() << std::endl;
				}
				catch (...)
				{
					response.set_status(503, "Service Unavailable");
					response.body += "We're sorry, but the server is currently unable to handle your request due to high load. Please try again later.";
				}
				// if (file_content.size() > max_size)
				// {
				// 	response.set_status(413, "Request Entity Too Large");
				// 	response.body += "The request entity is too large.";
				// }
				if (file.eof())
					break;
			}
			file.close();

			// response.body.swap(ostrm.str());
			response.content_length = std::to_string(file_content.length());
			// std::cout << "===" << response.content_length << std::endl;
			response.body.swap(file_content);
			// response.content_type = ;
		}
	}
};

class __POST
{
public:
	void handle(HTTP_request &request_info, HTTP_response &response, Mesage &msg)
	{
		file_info file;

		DataConf _server_ = lib.get_server_index(request_info, msg);
		file = lib.get_requested_file(request_info, _server_);
		// file = lib.get_requested_file(request_info.requested_file, msg._connections.second.first);
		// file = lib.get_requested_file(request_info.requested_file, msg._connections.second.first, request_info.method);
		// file = lib.get_requested_file();
		if (file._allowMeth[con_POST])
		{	

			if (file.is_redirect)
			{
                response.set_status(301, "Moved Permanently");
                response.location = file.location.__redirect.second;
				// std::cout << "waa nwaa3\n";
				// exit(0);
			}
			else
			{
				if (file.file_exists)
				{
					// if (file.is_file)
						// update_file(file, request_info, response);
					// else if (file.is_dir)
					// {
						// std::cout << request_info.content_type.first << std::endl;

						//handling cgi
						std::cout << file.location._cgi << std::endl;
						if (file.location._cgi)
						{
							// std::cout << "loc:" << file.location.__path << std::endl;
							_cgi_info cgi_info;
							create_env_(request_info, _server_, file);
							request_info.env_c = new char*[request_info.env_v.size() + 1];
							env_v_to_c(request_info.env_c, request_info.env_v);
							print_env(request_info.env_c, request_info.env_v.size());

							if (file_extention(file.file_path) == file.location.__cgi_ext)
							{
								std::cout << "run cgi\n";
								// cgi_info.cgi_name = file.file_path;
								// cgi_info.cgi_name = file.file_path;
								cgi_info.cgi_name = file.file_path;
								cgi_info.lang_path = file.location.__cgi_path;
								cgi_info.cgi_ext = file.location.__cgi_ext ;
								// cgi_info.lang_path = "/usr/local/bin/python3";
								// std::cout << "++++++++++++cgi path:" << cgi_info.cgi_path << std::endl;
								// cgi_info.cgi_ext = ".py";
								cgi(cgi_info , request_info, response);
							}
							else
							{
								response.set_status(403, "Forbidden extention for cgi");
							}
							free_env(request_info.env_c, request_info.env_v.size());
						}
						
						else if (request_info.content_type.first == "multipart/form-data")
						{
							deque<form_part> parts;
							// std::cout << "++++++>>>> " << request_info.content_type.second << "\n";
							if (check_for_end_boundary(request_info.body, request_info.content_type.second))
							{
								if(get_parts(request_info.body, request_info.content_type.second, parts))
								{
									std::cout << "we got to handle parts\n";
									//print parts

									handle_parts(file, parts, request_info, response);
									generat_response(parts, response);
								}
								else
								{
									response.set_status(400, "Bad Request1");
								}

							}
							else
							{
								response.set_status(400, "Bad Request2");
							}
						}
						else if (file.is_dir)
						{
							response.set_status(400, "Bad Request path");
						}
						else
							update_file(file, request_info, response);
					// }
					// else
						// generate_error(response, 800, "no idea on why i have this condition here");
				}
				else
				{
					creat_file(file, request_info, response);
				}
			}
		}
		else
		{
			response.set_status(405, "Method Not Allowed");
		}
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
		// file = lib.get_requested_file(request_info.requested_file, msg._connections.second.first);
		// file = lib.get_requested_file(request_info.requested_file, msg._connections.second.first, request_info.method);
		if (file._allowMeth[con_DELETE])
		{
			if (file.is_redirect)
			{
				std::cout << "waa nwaa3\n";
				exit(0);
			}
			else
			{
				if (file.file_exists)
				{
					if (file.is_file)
					{
						delete_file(file.file_path, response);
						
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
	//
	/// if the request is not chunked and the server received -1 then error
	//

	private:
		// string str_request;
		__GET GET_;
		__POST POST_;
		__DELETE DELETE_;
	public:

		handler() {}
		// handler(string re) : req(re){}
		// void handle(string re, string &res)
		void handle(Mesage &msg)
		{
			request req(msg.message);

			// req.request_checkpoint();
			// std::cout << "host:" << req.request_info.host << std::endl;
			// print("host:" + req.request_info.host);
			std::cout << "i got to handle\n";
			// if (req.request_info.host == lib._servers[msg._connections.second.first].__name + ":" + lib._servers[msg._connections.second.first].__port[msg._connections.second.second])
			// {
				std::cout << "i got to handle inside\n";
				if (req.request_info.method == GET)
					GET_.handle(req.request_info, req.response, msg);
				else if (req.request_info.method == POST)
					POST_.handle(req.request_info, req.response, msg);
				else if (req.request_info.method == DELETE)
					DELETE_.handle(req.request_info, req.response, msg);
			// }
			// else
			// {
				// req.response.set_status(400, "Bad Request it is not for this server");
			// }
			fill_response(req, msg.response);
			// std::cout << "response: " << msg.response << std::endl;
			// 	handle_delete();
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

		if (req.response.content_length != "")
		{std::cout << "got to content length\n"<< std::endl; res.append("Contnet-Length: "); res.append(req.response.content_length); res.append(CRLF);}

		if (req.response.content_type != "")
		{ res.append("Content-Type: "); res.append(req.response.content_type); res.append(CRLF);}
		
		if (req.response.location != "")
		{ res.append("Location: "); res.append(req.response.location); res.append(CRLF);}
		
		if (req.response.connection == "meeeeem")
		{ res.append("connection: "); res.append(req.response.connection); res.append(CRLF);}

		if (req.response.cookies != "")
		{ res.append("cookies: "); res.append(req.response.cookies); res.append(CRLF);}
		res.append(CRLF);
		res.append(req.response.body);
	}
};






