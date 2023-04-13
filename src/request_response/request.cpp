#include "../../inc/req_resp.hpp"
#include "../../inc/utils.hpp"

class request
{
	vector<string> start_line;
	vector<pair<string, string> > headers;
	string body;


public:

	HTTP_request request_info;
	HTTP_response response;

	request()
	{
		// start_line.resize(3, string(""));
		start_line.reserve(3);
		body = "";
	}

	request(string req)
	{
		start_line.reserve(3);
		body = "";
		parse(req);
	}

	void request_checkpoint()
	{
		validate_start_line();
		fill_req();
	}

	void parse(string req)
	{
		std::stringstream s_request(req);
		string tmp_str;
		string line;

		std::getline(s_request, line, '\r');
		std::stringstream start_l_s(line);
		std::getline(s_request, line, '\n');

		short i = 0;
		for (; i < 3 and std::getline(start_l_s, tmp_str, ' '); i++)
			start_line.push_back(tmp_str);

		if (i != 3)
			std::cout << "err msg\n";

		while (std::getline(s_request, tmp_str, '\n') and tmp_str != string("\r"))
		{
			size_t pos = tmp_str.find(":");
			headers.push_back(std::make_pair(string(tmp_str.substr(0, pos)), string(tmp_str.substr(pos + 1))));
		}
		if (tmp_str != "\r")
			std::cout << "err msg\n";
		while (std::getline(s_request, tmp_str))
			body.append(tmp_str);
		
		request_checkpoint();
	}

private:

	void set_status(short code, string text)
	{
		response.status_code = code;
		response.status_text = text;
	}
	
	short method_num()
	{
		if (start_line[METHOD] == "GET")
			return (GET);
		if (start_line[METHOD] == "POST")
			return (POST);
		if (start_line[METHOD] == "DELETE")
			return (DELETE);
	}

	// void get_accept_priority(vector<string> attr)
	// {
	// 	size_t pos;
	// 	string tmp_str;
	// 	for (vec_str_it it = attr.begin(), it != attr.end())
	// 	{
	// 		std::stringstream tmp(*it);

	// 		std::getline(tmp, tmp_str, ';');
	// 		pos = it->rfind("q=");
	// 		if (pos != it->npos)
	// 			request_info.accept.insert(std::make_pair(std::stof(*it + pos + 2), tmp_str));
	// 		else
	// 			request_info.accept.insert(std::make_pair(1, tmp_str));
	// 	}
	// }

	// void fill_accept(std::pair<string, string> header_pair)
	// {
	// 	std::stringstream accept_att(header_pair->second);
	// 	vector<string> tmp_;
	// 	int i = 0;

	// 	while (std::getline(accept_att, tmp_[i], ','))
	// 		i++;
	// 	get_accept_priority(accept_att);
	// }

	void fill_req()
	{
		request_info.method = method_num();
		request_info.requested_file = start_line[SOURCE];

		for(vec_strp_it it = headers.begin(); it != headers.end(); it++)
		{
			if (_to_lower(it->first) == "host")
			{
				if (request_info.host != "")
				{
					set_status(400, "bad request")
					return ;
				}
				request_info.host = it->second;
			}
			if (_to_lower(it->first) == "content-type")
			{
				if (request_info.content_type != "")
				{
					set_status(400, "bad request")
					return ;
				}
				fill_content_type(request_info.content_type , it->second);
				// request_info.content_type = it->second;
			}
			if (_to_lower(it->first) == "content-length")
			{
				if (request_info.content_length != -1)
				{
					set_status(400, "bad request")
					return ;
				}
				request_info.content_length = std::stoi(it->second);
				if (request_info.content_length < 0)
				{
					set_status(400, "bad request")
					return ;
				}
			}
			if (_to_lower(it->first) == "connection")
			{
				if (request_info.connection != "")
				{
					set_status(400, "bad request")
					return ;
				}
				request_info.connection = it->second;
			}
			// if (_to_lower(it->first) == "accept")
			// {
			//     fill_accept(*it);
			// }
		}
		request_info.body.swap(body);
	}

	void validate_start_line()
	{
		if (start_line.size() != 3){/*bad request*/}
		if (start_line[METHOD] == "GET" || start_line[METHOD] == "POST" || start_line[METHOD] == "DELETE")
		{
			/*
				// add a function to check is the method allowed for the requested source ;
				if ( methods.is_allowed(start_line[METHOD], req) == NOT_ALLOWED)
				{
					set_status(405, "Method Not Allowed");
					return ;
				}
				check for the source existence and accessibility ;
				methods.check_content_type(req, &reference) ;
			*/
			if (valid_http(start_line[SOURCE]) == false)
			{
				set_status(505, "HTTP Version Not Supported");
				return ;
			}
			
			// checking for source existence
			methods.handler(start_line[METHOD], req);

		   // check for source accessibility using sngat's config file for allowed paths and method (using the method class)
			// * to be done 
		}
		else
		{
			set_status(501, "Not Implemented");
		}
	}

	void print_req()
	{
		for (vec_str_it it = start_line.begin();it != start_line.end(); it++)
		{
			if (it + 1 == start_line.end())
				std::cout << *it << std::endl;
			else
				std::cout << *it << " ";
		}
		// std::cout << std::endl;

		for (vec_strp_it it = headers.begin(); it != headers.end(); it++)
			std::cout << it->first << ":" << it->second << std::endl;
		std::cout << std::endl;
		std::cout << std::endl << body;
	}

	void print_req(std::string req)
	{
		this->parse(req);
		this->print_req();
	}
};
