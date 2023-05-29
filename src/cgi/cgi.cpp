#include "../../inc/req_res.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

pid_t childpid;

void handleTimeout(int signum)
{
	(void)signum;
	kill(childpid, SIGKILL);
}

int cgi(_cgi_info cgi_info , HTTP_request &request_info, HTTP_response &response)
{
	const char* lang_path = cgi_info.lang_path.c_str();
	const char* cgi_name = cgi_info.cgi_name.c_str();
	char *av[] = {(char *) lang_path, (char *) cgi_name, nullptr};
	FILE *fp = tmpfile();
	int fd = fileno(fp);
	pid_t pid = fork();
	print_env(request_info.env_c, request_info.env_v.size());
	if (pid == -1)
	{
		std::cerr << "cgi error\n";
		response.set_status(500, "internal error");

	}
	if (pid == 0)
	{
		if (fd == -1)
		{
			perror("open");
			std::cerr << "error\n";
			exit(500);
		}
		if (dup2(fd, 1) == -1)
		{
			std::cerr << "dup error\n";
			exit(500);
		}
		execve(lang_path, av, request_info.env_c);
		perror("execve"); // Print error message
		close(fd);
		exit(500);
	}
	childpid = pid;
	signal(SIGALRM, handleTimeout);
    alarm(5);
	int status;
    waitpid(pid, &status, 0);
	alarm(0);
	int signalNumber = WTERMSIG(status);
	if (signalNumber > 0)
	{
		response.body = "script took too long to execute.";
		response.content_length = response.body.size();
		response.set_status(200, "OK");
		response.content_type = "text/html";
		return 1;
	}
	long fileSize = ftell(fp);
	response.content_length = fileSize;
	response.set_status(200, "OK");
	response.content_type = "text/html";
	rewind(fp);
	char readFromFile[fileSize + 1];
	fread(readFromFile, 1, fileSize, fp);
	response.body = readFromFile;
	return 1;
}
