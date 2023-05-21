#ifndef EVERYTHING_HPP
# define EVERYTHING_HPP
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <iostream>

int cgi(std::string text);
std::string set_cookies(std::string name, std::string value);


#endif