/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shilal <shilal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:50:23 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/22 20:41:25 by shilal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location(Server &serv){
    path = "";
    methods = "";
	rediractionURL = "";
	rediractionStatusCode = 0;
    root = serv.getRoot();
    upload = serv.getUpload();
    uploadPath = serv.getUploadPath();
    autoIndex = serv.getAutoIndex();
	errorPages = serv.getErrorPages();
}

Location::~Location(){}

std::string Location::getRediractionURL() {
    return rediractionURL;
}

int Location::getRediractionStatusCode() {
    return rediractionStatusCode;
}

std::map<int, std::string> &Location::getErrorPages(){
	return errorPages;
}

std::string Location::getErrorPagesByKey(int key) {
	if (errorPages.find(key) != errorPages.end())
		return errorPages[key];
	return "";
}
std::map<std::string, std::string> &Location::getCgiPaths() {
	return cgiPaths;
}

std::string Location::getCgiByKey(std::string key) {
	if (cgiPaths.find(key) != cgiPaths.end())
		return cgiPaths.at(key);
	return "";
}

std::vector<std::string> &Location::getIndex() {
	return index;
}

std::string Location::getAutoIndex() {
    return autoIndex;
}

std::string Location::getUpload() {
    return upload;
}

std::string Location::getUploadPath() {
    return uploadPath;
}

std::string Location::getRoot() {
    return root;
}

std::string Location::getPath() {
    return path;
}

std::string Location::getMethods() {
    return methods;
}

void Location::setRoot(std::string value) {
	std::ifstream ifs;

	size_t pos = value.find_last_not_of(" ");
	if (pos != std::string::npos)
		value = value.substr(0, pos + 1);
	if (!isDir(value))
		throw std::runtime_error("root : invalide value " + value);
	ifs.open(value.c_str());
	if (!ifs.is_open())
		throw std::runtime_error("root : invalide value " + value);
	ifs.close();
	root = value;
}

void Location::setPath(std::string value) {
	size_t pos = value.find_last_not_of(" ");
	if (pos != std::string::npos)
		value = value.substr(0, pos + 1);
	path = value;
}

void Location::setMethods(std::string value) {
	std::stringstream ss(value);
	std::string token;
	methods = "";
	
	while (ss >> token) {
		if (token != "POST" && token != "GET" && token != "DELETE")
			throw std::runtime_error("Method : " + token + " not valide");
		else if (!methods.empty() && methods.find(token) != std::string::npos)
			continue ;
		else
			methods += token + " ";
	}
	if (methods.empty())
		throw std::runtime_error("methods : undifind value " + value);
}


void Location::setRediraction(std::string value) {
	std::string statusCode;
	
	size_t pos = value.find_last_not_of(" ");
	if (pos != std::string::npos)
		value = value.substr(0, pos + 1);
	
	pos = value.find(" ");
	if (pos == std::string::npos)
		throw std::runtime_error("return : invalide value " + value);
	statusCode = value.substr(0, pos);
	if (statusCode.length() != 3 || statusCode.find_first_not_of("0123456789") != std::string::npos)
		throw std::runtime_error("return : invalid value " + value);

	rediractionStatusCode = atoi(statusCode.c_str());
	if (!HttpStatus::isRedirection(rediractionStatusCode))
		throw std::runtime_error("return : invalide rediraction Status Code " + statusCode);

	rediractionURL = value.substr(3);
	pos = rediractionURL.find_first_not_of(" ");
	if (pos != std::string::npos)
		rediractionURL = rediractionURL.substr(pos);
}

void Location::setAutoIndex(std::string value) {
	size_t pos = value.find_last_not_of(" ");
	if (pos != std::string::npos)
		value = value.substr(0, pos + 1);
	if (value == "on" || value == "ON" || value == "On")
    	autoIndex = "on";
	else if (value == "OFF" || value == "off" || value == "Off")
    	autoIndex = "off";
	else
		throw std::runtime_error("autoindex : invalide value");
}

void Location::setUpload(std::string value) {
	std::stringstream ss;
	std::string token;
	std::string path;
	std::ifstream ifs;

	size_t pos = value.find_last_not_of(" ");
	if (pos != std::string::npos)
		value = value.substr(0, pos + 1);

	ss << value;
	ss >> token;
	// get token
	if (token == "on" || token == "ON" || token == "On")
		upload = "on";
	else if (token == "OFF" || token == "off" || token == "Off")
    	upload = "off";
	else
		throw std::runtime_error("upload : invalide value " + value);
	// get path
	getline(ss, path, '\0');
	pos = path.find_first_not_of(" ");
	if (pos != std::string::npos)
		path = path.substr(pos);
	if (path.empty())
		return ;
	if (!isDir(path))
		throw std::runtime_error("upload : invalide path " + path);
	ifs.open(path.c_str());
	if (!ifs.is_open())
		throw std::runtime_error("upload : invalide path " + path);
	ifs.close();
	uploadPath = path;
}

void Location::setIndex(std::string value) {
	std::stringstream ss;
	std::string token;

	ss << value;
	while (ss >> token) {
		if (std::find(index.begin(), index.end(), token) == index.end())
			index.push_back(token);
	}
}

void Location::insertErrorPages(std::string line, std::string value) {
	std::stringstream ss;
	std::ifstream ifs;
	std::string token;
	std::string path;
	int statusCode;

	size_t pos = value.find_last_not_of(" ");
	if (pos != std::string::npos)
		value = value.substr(0, pos + 1);
	pos = value.find_last_of(",");
	if (pos == std::string::npos)
		throw std::runtime_error("error_pages : invalid value " + line);
	path = value.substr(pos + 1, value.length() - pos);
	value = value.substr(0, pos);
	pos = path.find_first_not_of(" ");
	if (pos != std::string::npos)
		path = path.substr(pos);

	if (!isRegFile(path))
		return ;
	ifs.open(path.c_str());
	if (!ifs.is_open())
		return ;
	ifs.close();

	pos = value.find_first_not_of(" ");
	if (pos == std::string::npos)
		throw std::runtime_error("error_pages : invalid value " + line);
	value = value.substr(pos);

	ss << value;

	while (ss >> token) {
		if (token.length() != 3 || token.find_first_not_of("0123456789") != std::string::npos || !HttpStatus::isVadilCode(atoi(token.c_str())))
			throw std::runtime_error("error_pages : invalide Status Code " + token);
		statusCode = atoi(token.c_str());
		if (errorPages.find(statusCode) != errorPages.end())
			errorPages.at(statusCode) = path;
		else
			errorPages.insert(std::make_pair(statusCode, path));
	}
}

void Location::setErrorPages(std::string value) {
	std::string token;
	std::string str;
	
	size_t pos = value.find_last_not_of(" ");
	if (pos != std::string::npos)
		str = value.substr(0, pos + 1);
	if (str.empty() || str.size() < 3 || str.at(0) != '[' || str.at(str.size() - 1) != ']')
		throw std::runtime_error("error_pages : invalid foramt " + value);
	
	str = str.substr(1, str.length() - 2);

	while (!str.empty()) {
		if (str.at(0) != '[')
			throw std::runtime_error("error_pages : invalid foramt " + value);
		
		pos = str.find(']');
		if (pos == std::string::npos)
			throw std::runtime_error("error_pages : invalid foramt " + value);
		else {
			token = str.substr(1, pos - 1);
			str = str.substr(pos + 1);
			insertErrorPages(value, token);
			pos = str.find_first_not_of(" ,");
			if (pos == std::string::npos) {
				if (str.find(",") != std::string::npos)
					throw std::runtime_error("error_pages : invalid foramt " + value);
				break;
			}
			if (str.substr(0, pos).find(",") == std::string::npos)
				throw std::runtime_error("error_pages : invalid foramt " + value);
			str = str.substr(pos);
		}
	}
}

void Location::insertCgiPath(std::string line, std::string value) {
	std::stringstream ss;
	std::string token;
	std::string path;

	size_t pos = value.find_last_not_of(" ");
	if (pos != std::string::npos)
		value = value.substr(0, pos + 1);
	pos = value.find_last_of(",");
	if (pos == std::string::npos)
		throw std::runtime_error("cgi_paths : invalid value " + line);
	path = value.substr(pos + 1, value.length() - pos);
	value = value.substr(0, pos);
	pos = path.find_first_not_of(" ");
	if (pos != std::string::npos)
		path = path.substr(pos);
	pos = value.find_first_not_of(" ");
	if (pos == std::string::npos)
		throw std::runtime_error("cgi_paths : invalid value " + line);
	value = value.substr(pos);
	
	ss << value;
	ss >> token;
	
	if (cgiPaths.find(token) != cgiPaths.end())
		cgiPaths.at(token) = path;
	else
		cgiPaths.insert(std::make_pair(token, path));
	
	token = "";
	ss >> token;
	
	if (!token.empty())
		throw std::runtime_error("cgi_paths : invalid value " + line);
}

void Location::setCgiPath(std::string value) {
	std::string token;
	std::string str;
	
	size_t pos = value.find_last_not_of(" ");
	if (pos != std::string::npos)
		str = value.substr(0, pos + 1);
	if (str.empty() || str.size() < 3 || str.at(0) != '[' || str.at(str.size() - 1) != ']')
		throw std::runtime_error("cgi_paths : invalid foramt " + value);
	
	str = str.substr(1, str.length() - 2);

	while (!str.empty()) {
		if (str.at(0) != '[')
			throw std::runtime_error("cgi_paths : invalid foramt " + value);
		
		pos = str.find(']');
		if (pos == std::string::npos)
			throw std::runtime_error("cgi_paths : invalid foramt " + value);
		else {
			token = str.substr(1, pos - 1);
			str = str.substr(pos + 1);
			insertCgiPath(value, token);
			pos = str.find_first_not_of(" ,");
			if (pos == std::string::npos) {
				if (str.find(",") != std::string::npos)
					throw std::runtime_error("cgi_paths : invalid foramt " + value);
				break;
			}
			if (str.substr(0, pos).find(",") == std::string::npos)
				throw std::runtime_error("cgi_paths : invalid foramt " + value);
			str = str.substr(pos);
		}
	}
}

void Location::checkLocation() {
	if (path.empty())
		throw std::runtime_error("location importent data : path | methods ...");
}

void Location::initEmptyData(Server &serv) {
	if (methods.empty())
		methods = serv.getMethods();
	if (index.empty()) {
		for (std::vector<std::string>::iterator it = serv.getIndex().begin(); it != serv.getIndex().end(); it++) {
			if (std::find(index.begin(), index.end(), *it) == index.end())
				index.push_back(*it);
		}
	}
	else {
		std::ifstream ifs;
		std::string path;
		for (std::vector<std::string>::iterator it = index.begin(); it != index.end(); it++) {
			if (root.at(root.length() - 1) != '/')
				path = root + "/" + *it;
			else
				path = root + *it;
			if (!isRegFile(path)) {
				it->erase();
				continue ;
			}
			ifs.open(path.c_str());
			if (!ifs.is_open()) {
				it->erase();
				continue ;
			}
			ifs.close();
		}
	}
}

// void Location::checkLocation(){
    
//     for (std::map<std::string, std::string>::iterator it = arg.begin() ; it != arg.end(); it++) {
//         if (it->second.empty())
//             throw std::runtime_error("location importent data : path | methods ...");
//     }
// }

void Location::printArg() {
    std::cout << "            path : *" << getPath() << "*" << std::endl;
    std::cout << "            root : *" << getRoot() << "*"  << std::endl;
    std::cout << "            methods : *" << getMethods() << "*"  << std::endl;
    std::cout << "            upload : *" << getUpload() << "*"  << std::endl;
    std::cout << "            upload Path : *" << getUploadPath() << "*" << std::endl;
	std::cout << "            autoIndex : *" << getAutoIndex() << "*"  << std::endl;
	std::cout << "            rediraction : *" << getRediractionStatusCode() << " " << getRediractionURL() << "*"  << std::endl;
	if (!getIndex().empty()) {
		std::cout << "            index : ";
		for (std::vector<std::string>::iterator it = getIndex().begin(); it != getIndex().end(); it++) {
			std::cout << *it << " ";
		}
		std::cout << std::endl;
	}
	if (!getErrorPages().empty()) {
		std::cout << "            errorpages : " << std::endl;
		for (std::map<int, std::string>::iterator it = getErrorPages().begin(); it != getErrorPages().end(); it++) {
			std::cout << "                - " << (*it).first << " : " << (*it).second << std::endl;
		}
	}
	if (!getCgiPaths().empty()) {
		std::cout << "            cgi_paths : " << std::endl;
		for (std::map<std::string, std::string>::iterator it = getCgiPaths().begin(); it != getCgiPaths().end(); it++) {
			std::cout << "                - " << (*it).first << " : " << (*it).second << std::endl;
		}
	}
}

