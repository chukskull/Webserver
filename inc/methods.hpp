#include "req_res.hpp"
#include "../src/request_response/request.cpp"
#include "Library.hpp"

class __GET
{
	__GET() {}

public:

	void handle(HTTP_request &request_info, HTTP_response &response)
	{
		file_info file;

		file = lib.get_requested_file(request_info.requested_file);

		if (request_info.connection == "close")
			response.connection = CLOSE_CONNECTION;
		else
			response.connection = KEEP_ALIVE;

		if (file._allowMeth[con_GET])
		{
			if (file.is_redirect)
			{
			}
			else if (file.is_dir)
			{
				if (file.is_autoindex)
				{
					generate_autoindex(file, response);
					response.set_status(200, "OK");
					// serve autoindex
					// create the html page
				}
				else
				{
				}
			}
			else if (file.is_file)
			{
				if (file.file_exist)
				{
					response.set_status(200, "OK");
					response.content_type = file.content_type;
					read_file(file.file_path, response);
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
			response.set_status(404, "file not found");
			response.body += "The requested file was not found on this server.";
			// return ;
		}
		else
		{
			string buffer;
			size_t max_size = MAX_READ_SIZE;

			// max_size = set_max_size(host) ;
			buffer.resize(max_size);
			while (file.read(buffer.c_str() , max_size))
			{
				try
				{
					file_content += buffer;
				}
				catch (...)
				{
					response.set_status(503, "Service Unavailable");
					response.body += "We're sorry, but the server is currently unable to handle your request due to high load. Please try again later.";
				}
				if (file_content.size() > max_size)
				{
					response.set_status(413, "Request Entity Too Large");
					response.body += "The request entity is too large.";
				}
				if (file.eof())
					break;
			}
			file.close();
			response.content_length = file_content.size();
			response.body.swap(file_content);
			response.content_type = ;
		}
	}
};

class __POST
{
	void handle(HTTP_request &request_info, HTTP_response &response)
	{
		file_info file;

		file = lib.get_requested_file();
		if (file._allowMeth[con_POST])
		{	
			if (file.is_redirect)
			{

			}
			else
			{
				if (file.file_exists)
				{
					if (file.is_file)
						update_file(file, request_info, response);
					if (file.is_dir)
					{
						if (request_info.content_type.first == "multipart/form-data")
						{
							deque<form_part> parts;
							if (check_for_end_boundary(request_info.body, request_info.content_type.second))
							{
								if(get_parts(request_info.body, request_info.content_type.second, parts))
								{
									handle_parts(parts, response);
									generat_response(parts, response);
								}
								else
								{
									response.set_status(400, "Bad Request");
								}

							}
							else
							{
								response.set_status(400, "Bad Request");
							}
						}
					}
					else
						generate_error();
				}
				else
				{
					creat_file(file, request_info, response);
				}
			}
		}
	}
};

class __DELETE
{

};

class handler 
{
	private:
		string request;
		request req;
		__GET GET_;
		__POST POST_;
	public:
		handler() {}
		handler(string req) : request(req) {}
		void handle()
		{
			req.request_checkpoint();
			if (req.request_info.method == GET)
				GET_.handle();
			else if (req.request_info.method == POST)
				POST_.handle();
			// else if (req.request_info.method == DELETE)
			// 	handle_delete();
		}
};

