/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 00:44:56 by shilal            #+#    #+#             */
/*   Updated: 2024/04/26 23:19:04 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

void Request::RemoveContentDir(std::string str){
	
	struct dirent* dr;
	
	if (str[str.length() - 1] != '/')
		str += "/";
	DIR* dir = opendir(str.c_str());
	if (!dir)
		throw StatusCodeExcept(403);
	while ((dr = readdir(dir))){
		std::string name = dr->d_name;
		if (name != "." && name != "..")
		{
			if (isDir(str + name) == true)
				RemoveContentDir(str + name);
			else {
				if (std::remove((str + name).c_str()) == -1){
					closedir(dir);
					throw StatusCodeExcept(500);
				}
			}
		}
	}
	if (std::remove((str).c_str()) == -1){
		closedir(dir);
		throw StatusCodeExcept(500);
	}
	closedir(dir);
}

void Request::Delete(){

    struct stat buffer;
    int st = stat(url.c_str(), &buffer);
    if (st == -1)
        throw StatusCodeExcept(404);
    if (url == location->getRoot() + "/")
        throw StatusCodeExcept(403);
    
    if (S_ISDIR(buffer.st_mode)){
        if (location->getRediractionStatusCode() != 0)
            throw rediractionExcept(location->getRediractionStatusCode(), location->getRediractionURL());
        if (!reqURL.empty() && reqURL[reqURL.size() - 1] != '/')
            throw rediractionExcept(302, reqURL + "/" + queryString);

        if (access(url.c_str(), W_OK) == -1)
            throw StatusCodeExcept(403);
        RemoveContentDir(url);
    }
    else {
        if (std::remove(url.c_str()) == -1)
            throw StatusCodeExcept(403);
    }
    throw StatusCodeExcept(204);
}