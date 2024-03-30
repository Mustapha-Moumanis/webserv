/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shilal <shilal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 00:25:46 by shilal            #+#    #+#             */
/*   Updated: 2024/03/30 00:40:04 by shilal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

void Request::rediractionCGI(){

	struct stat buffer;
	std::cout << url << std::endl;
	int st = stat(url.c_str(), &buffer);
	if (st == -1)
		throw StatusCodeExcept(201);
	if (location->getRediractionStatusCode() != 0)
		throw rediractionExcept(location->getRediractionStatusCode(), location->getRediractionURL());
	if (!S_ISDIR(buffer.st_mode)){
		size_t found = url.find_last_of(".");
		if (found != std::string::npos){
			std::string extention = url.substr(found);
			std::map<std::string, std::string> cgiPath = location->getCgiPaths();
			std::map<std::string, std::string>::iterator it = cgiPath.find(extention);
			if (it != cgiPath.end()){
				std::cout << "ITS TIME TO CGI" << std::endl;
				cgitest(fileno(ftype), it->second, url.substr(url.find_last_of("/")+1));
			}
		}
	}
	throw StatusCodeExcept(201);
}

void Request::cgitest(int fd, std::string path, std::string methode){

	(void)methode;
	std::string script = "SCRIPT_NAME=" + url;

	char *envp[] = {
		(char*)"CONTENT_LENGTH=0",
		(char*)"CONTENT_TYPE=text/html",
		(char*)"REDIRECT_STATUS=true",
		(char*) script.c_str(),
		(char*)"SERVER_PROTOCOL=HTTP/1.1",
		NULL
	};

    char *argv[] = {
		(char*) path.c_str(),
        (char*) url.c_str(), 
        NULL
    };


	FILE *type = fopen("cgi.txt", "w+");
	if (type == NULL) 
		throw StatusCodeExcept(403);
	int f = fileno(type);
    pid_t p;
    p = fork();
    if(p < 0) {
      perror("fork fail");
      exit(1);
    }
    else if ( p == 0){
        dup2(fd, 0);
		dup2(f, 1);
		dup2(f, 2);
       	execve(path.c_str(), argv, envp);
		std::cerr << "Failed to execute PHP script" << std::endl;
		exit (1);// #include <unistd.h>
    }
	else {
		int	status;
		waitpid(p, &status, 0);
		kill(p,9);
		if (WIFEXITED(status)){
			if (WEXITSTATUS(status) != 0)
				throw StatusCodeExcept(500);
		}
		// parssRspCGI();
	}
	fclose(type);
}