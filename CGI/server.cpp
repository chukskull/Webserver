#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "hd.h"

int user = 0;

int main() {
    // Create a socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    //reuse addr
    int optval = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    // Bind the socket to a local address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8040);
   if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)))
   {
        std::cout << "failed to bind\n";
        return 0;
   }

    // Listen for incoming connections
    listen(server_socket, 1);

    // Accept incoming connections and handle them
    while (true) {
        // Accept a connection
        struct sockaddr_in client_address;
        socklen_t client_address_size = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_size);
        
        // Read the request from the client
        char buffer[1000000] = {0};
        read(client_socket, buffer, 1000000);
        std::cout  << buffer <<std::endl;

        std::istringstream request_stream(buffer);
        std::string request_line;
        getline(request_stream, request_line);
        std::istringstream request_line_stream(request_line);
        std::string method, path, http_version;
        request_line_stream >> method >> path >> http_version;

        // Check if the requested file exists and is an HTML file
        std::ifstream file(path.substr(1));
        if (path == "/")
        {
            file.close();
            file.open("index.html");
        }
        if (path == "/cgi")
        {
            std::string line, last, text;
            while(getline(request_stream, line))
                last = line;
            text = last.substr(5);
            cgi(text);
            std::ifstream ifs("cgi/respons.txt");
            std::stringstream response_str;
            std::string response_stream;
            std::string tr;
            response_str << ifs.rdbuf();
            tr = response_str.str();
            response_stream+= "HTTP/1.1 200 ok\r\n\r\n";
            response_stream+= "<html><body><h1> length is : ";
            response_stream+= tr;
            response_stream+=  "</h1></body></html>";
            send(client_socket, response_stream.c_str(), strlen(response_stream.c_str()), 0);
        
        }
        else if (file.good())
        {
            // If the file exists and is an HTML file, send the file contents as the response
            std::ostringstream response_stream;
            response_stream << "HTTP/1.1 200 ok\r\n";
            response_stream << set_cookies("rock", "you");
            response_stream << "\r\n";
            std::string line;
            while (getline(file, line))
                response_stream << line << std::endl;
            std::string response = response_stream.str();
            // std::cout << str << std::endl;
            write(client_socket, response.c_str(), response.size());
        }
        else
        {
            std::ostringstream response_stream;
            // If the file does not exist or is not an HTML file, send a 404 response
            response_stream << "HTTP/1.1 404 Not Found hhhhhh\n\n";
            response_stream << "<html><body><h1>404 Not Found</h1></body></html>";
            std::string response = response_stream.str(); 
            // std::cout << response << std::endl;
            write(client_socket, response.c_str(), response.size());
        }

        // Close the client socket
        close(client_socket);
    }

    // Close the server socket
    close(server_socket);

    return 0;
}
