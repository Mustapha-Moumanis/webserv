/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shilal <shilal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 00:25:46 by shilal            #+#    #+#             */
/*   Updated: 2024/03/31 23:53:22 by shilal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

void Request::rediractionCGI(){

	struct stat buffer;
	int st;
	if ((st = stat(url.c_str(), &buffer)) != -1){
		if (location->getRediractionStatusCode() != 0)
			throw rediractionExcept(location->getRediractionStatusCode(), location->getRediractionURL());
		if (S_ISREG(buffer.st_mode)){
			size_t found = url.find_last_of(".");
			if (found != std::string::npos){
				std::string extention = url.substr(found);
				std::map<std::string, std::string> cgiPath = location->getCgiPaths();
				std::map<std::string, std::string>::iterator it = cgiPath.find(extention);
				if (it != cgiPath.end()){
					std::cout << "IT'S TIME TO CGI" << std::endl;
					fseek(ftype, 0, SEEK_SET);
					cgitest(fileno(ftype), it->second, url.substr(url.find_last_of("/")+1));
				}
			}
		}
	}
	throw StatusCodeExcept(201);
}

void Request::parssRspCGI(FILE *type){

	char buffer[1024] = {0};
	int j = fread(buffer, sizeof(buffer[0]), 1024, type);
	std::string str(buffer, j);
	size_t pos = str.find("\r\n\r\n");
	if (pos == std::string::npos)
		pos = str.find("\n\n");
	if (pos != std::string::npos){
		std::string header = str.substr(0, pos);
		std::string body = str.substr(pos + 4);
		std::cout << header << std::endl;
		std::cout << "this is body" << std::endl;
		std::cout << body << std::endl;
	}
	else{
		std::cout << "this is py" << std::endl;
		// generate header
	}
}

void Request::cgitest(int fd, std::string path, std::string methode){

	methode = "REQUEST_METHOD=" + methode;
	std::string script = "SCRIPT_NAME=" + url;
	std::string scriptFile = "SCRIPT_FILENAME=" + url;
	std::string contentLength = "CONTENT_LENGTH=" + this->length;
	std::string contentType = "CONTENT_TYPE=";

	char *envp[] = {
		// (x > y) ? : ;
		(char*) contentLength.c_str(),
		(char*)"CONTENT_TYPE=application/x-www-form-urlencoded",
		(char*)"REDIRECT_STATUS=true",
		(char*) script.c_str(),
		(char*) scriptFile.c_str(),
		(char*) methode.c_str(),
		(char*)"SERVER_PROTOCOL=HTTP/1.1",
		NULL
	};

    char *argv[] = {
		(char*) path.c_str(),
        (char*) url.c_str(), 
        NULL
    };


	FILE *type = fopen("cgi.txt", "wb+");
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
		// dup2(f, 2);
       	execve(path.c_str(), argv, envp);
		std::cerr << "Failed to execute PHP script" << std::endl;
		exit (1);
    }
	else {
		int	status;
		waitpid(p, &status, 0);
		kill(p,9);
		if (WIFEXITED(status)){
			if (WEXITSTATUS(status) != 0)
				throw StatusCodeExcept(500);
		}
	}
	fseek(type, 0, SEEK_SET);
	parssRspCGI(type);
	fclose(type);
}