/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shilal <shilal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 00:25:46 by shilal            #+#    #+#             */
/*   Updated: 2024/04/22 16:34:12 by shilal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

void Request::parssRspCGI(){

	char buffer[1024] = {0};
	int j = fread(buffer, sizeof(buffer[0]), 1024, fCgi);
	std::string str(buffer, j);
	
	size_t pos = str.find("\r\n\r\n");
	if (pos != std::string::npos){
		str = str.substr(0, pos + 4);
		std::string header = str.substr(0, pos + 4);
		while (!header.empty()) {
			std::string key = header.substr(0, header.find(": "));
			header.erase(0, header.find(": ") + 2);
			if (key == "Status"){
				std::string val = header.substr(0, header.find("\r\n"));
				header = "HTTP/1.1 " + val + "\r\n";
				break ;
			}
			header.erase(0, header.find("\r\n") + 2);
		}
		if (header.empty())
			header = "HTTP/1.1 200 OK\r\n";
		header += str;
		throw responseGetExcept(header, fileCgiName, _FILE, pos + 4);
	}
	else 
		throw responseGetExcept(genGetDirHeader(200, "text/html"), fileCgiName, _FILE, 0);
}

void Request::cgiPost(int fd, std::string path){

	*ptrTime = clock();

	std::string script = "SCRIPT_NAME=" + url;
	std::string scriptFile = "SCRIPT_FILENAME=" + url;
	std::string ContentType = "CONTENT_TYPE=" + this->contentType;
	
	std::stringstream ss;
	std::string len;
	ss << this->length;
	ss >> len;
	std::string ContentLength = "CONTENT_LENGTH=" + len;
	
	std::string cookie = "HTTP_COOKIE=";
	if (!cookies.empty())
		cookie += cookies;

	char *envp[] = {
		(char*) ContentLength.c_str(),
		(char*) ContentType.c_str(),
		(char*) script.c_str(),
		(char*) scriptFile.c_str(),
		(char*) "REQUEST_METHOD=POST",
		(char*)"REDIRECT_STATUS=true",
		(char*)"SERVER_PROTOCOL=HTTP/1.1",
		NULL
	};

    char *argv[] = {
		(char*) path.c_str(),
        (char*) url.c_str(), 
        NULL
    };

	fileCgiName = getNewName() + "Cgi.txt";
	fCgi = fopen(fileCgiName.c_str(), "wb+");
	if (fCgi == NULL)
		throw StatusCodeExcept(403);
	int f = fileno(fCgi);
    p = fork();
    if (p < 0)
      	throw StatusCodeExcept(403);
    else if ( p == 0){
        dup2(fd, 0);
		dup2(f, 1);
       	execve(path.c_str(), argv, envp);
		std::cerr << "Failed to execute script" << std::endl;
		exit (1);
    }
	int	status;
	int j = waitpid(p, &status, WNOHANG);
	if (j != 0) {
		kill(p,9);
		waitpid(p, NULL, 0);
		if (WIFEXITED(status)){
			if (WEXITSTATUS(status) != 0)
				throw StatusCodeExcept(500);
		}
		fseek(fCgi, 0, SEEK_SET);
		parssRspCGI();
	}
	else
		*ptrIsCgi = 1;
}

void Request::cgiGet(std::string path, std::string url){

	*ptrTime = clock();
	
	std::string script = "SCRIPT_NAME=" + url;
	std::string scriptFile = "SCRIPT_FILENAME=" + url;
	std::string ContentType = "CONTENT_TYPE=" + this->contentType;

	if (!queryString.empty())
		queryString = queryString.substr(1);
	std::string query = "QUERY_STRING=" + queryString;

	std::string cookie = "HTTP_COOKIE=";
	if (!cookies.empty())
		cookie += cookies;

	char *envp[] = {
		(char*) query.c_str(),
		(char*) ContentType.c_str(),
		(char*) script.c_str(),
		(char*) scriptFile.c_str(),
		(char*) cookie.c_str(),
		(char*) "REQUEST_METHOD=GET",
		(char*) "REDIRECT_STATUS=true",
		(char*) "SERVER_PROTOCOL=HTTP/1.1",
		NULL
	};

    char *argv[] = {
		(char*) path.c_str(),
        (char*) url.c_str(), 
        NULL
    };

	fileCgiName = getNewName() + "Cgi.txt";
	fCgi = fopen(fileCgiName.c_str(), "wb+");
	if (fCgi == NULL) 
		throw StatusCodeExcept(403);
	int fd = fileno(fCgi);
    p = fork();
    if (p < 0)
      throw StatusCodeExcept(403);
    else if ( p == 0){
		dup2(fd, 1);
       	execve(path.c_str(), argv, envp);
		std::cerr << "Failed to execute script" << std::endl;
		exit (1);
    }
	int	status;
	int j = waitpid(p, &status, WNOHANG);
	if (j != 0) {
		kill(p,9);
		waitpid(p, NULL, 0);
		if (WIFEXITED(status)){
			if (WEXITSTATUS(status) != 0)
				throw StatusCodeExcept(500);
		}
		fseek(fCgi, 0, SEEK_SET);
		parssRspCGI();
	}
	else
		*ptrIsCgi = 1;
}