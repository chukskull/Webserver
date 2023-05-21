#include "../hd.h"



int cgi(std::string text)
{
	char *av[] = {nullptr, nullptr, nullptr};
	const char *cstr;
	cstr = text.c_str();
	av[1] = new char[text.length() + 1];
	std::strcpy(av[1], cstr);
	av[0] = new char[strlen("cgi/cgi-script") + 1];
	std::strcpy(av[0], "cgi/cgi-script");
	pid_t pid = fork();
	if (pid == -1)
	{
		std::cerr << "cgi error\n";
		return(1);
	}
	if (pid == 0)
	{
		int out = open("cgi/respons.txt",O_RDWR | O_TRUNC | O_CREAT, 0777);
		if (out == -1)
		{
			perror("open");
			std::cerr << "error\n";
			return 0;
		}
		if (dup2(out, 1) == -1)
		{
			std::cerr << "dup error\n";
			exit(1);
		}
		close(out);
		execve("cgi/cgi-script", av, NULL);
		perror("execve"); // Print error message
		exit(0);
	}
	wait(NULL);
	return 1;
}
