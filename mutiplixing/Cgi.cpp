/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shilal <shilal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 00:25:46 by shilal            #+#    #+#             */
/*   Updated: 2024/04/02 00:31:52 by shilal           ###   ########.fr       */
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
					fseek(ftype, 0, SEEK_SET);
					cgiPost(fileno(ftype), it->second);
				}
			}
		}
	}
}

void Request::parssRspCGI(FILE *type){

	char buffer[1024] = {0};
	int j = fread(buffer, sizeof(buffer[0]), 1024, type);
	std::string str(buffer, j);
	size_t pos = str.find("\r\n\r\n");
	if (pos != std::string::npos){
		std::string header = str.substr(0, pos + 2);
		while (!header.empty()) {
			std::string key = header.substr(0, header.find(": "));
			header.erase(0, header.find(": ") + 2);
			std::string val = header.substr(0, header.find("\r\n"));
			HeaderCgi.insert(std::pair<std::string,std::string>(key, val));
			header.erase(0, header.find("\r\n") + 2);
		}
		for (std::map<std::string, std::string>::iterator it = HeaderCgi.begin(); it != HeaderCgi.end(); it++){
			std::cout << it->first << " " << it->second << std::endl;
 		}
	}
	else{
		std::cout << "this is py" << std::endl;
		std::cout << str << std::endl;
		// generate header
	}
}


void Request::cgiPost(int fd, std::string path){

	std::string script = "SCRIPT_NAME=" + url;
	std::string scriptFile = "SCRIPT_FILENAME=" + url;
	std::string ContentType = "CONTENT_TYPE=" + this->contentType;
	
	std::stringstream ss;
	std::string len;
	ss << this->length;
	ss >> len;
	std::string ContentLength = "CONTENT_LENGTH=" + len;

	std::cout << script << std::endl;
	std::cout << scriptFile << std::endl;
	std::cout << ContentLength << std::endl;
	std::cout << ContentType << "\n" << std::endl;

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

	
	FILE *type = fopen("cgi.txt", "wb+");
	if (type == NULL) 
		throw StatusCodeExcept(403);
	int f = fileno(type);
    pid_t p;
    p = fork();
    if (p < 0)
      throw StatusCodeExcept(403);
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
	std::remove(fileName.c_str());
}

void Request::cgiGet(std::string path){

	std::string script = "SCRIPT_NAME=" + url;
	std::string scriptFile = "SCRIPT_FILENAME=" + url;
	std::string ContentType = "CONTENT_TYPE=" + this->contentType;
	std::string query = "QUERY_STRING=" + queryString;

	// std::cout << script << std::endl;
	// std::cout << scriptFile << std::endl;
	// std::cout << query << std::endl;
	// std::cout << ContentType << std::endl;
	
	char *envp[] = {
		(char*) query.c_str(),
		(char*) ContentType.c_str(),
		(char*) script.c_str(),
		(char*) scriptFile.c_str(),
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

	
	FILE *type = fopen("cgi.txt", "wb+");
	if (type == NULL) 
		throw StatusCodeExcept(403);
	int fd = fileno(type);
    pid_t p;
    p = fork();
    if (p < 0)
      throw StatusCodeExcept(403);
    else if ( p == 0){
		dup2(fd, 1);
		// dup2(fd, 2);
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