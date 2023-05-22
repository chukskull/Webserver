#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <iostream>

int main(int ac, char **av)
{
	int len = 0;
	while(av[1][len])
		len ++;
	std::cout << len;
	return 0;
}