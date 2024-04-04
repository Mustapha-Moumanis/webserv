/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 14:31:08 by mmoumani          #+#    #+#             */
/*   Updated: 2024/04/04 23:39:07 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdlib>
#include <climits>
#include "Location.hpp"
#include "../include/Utils.hpp"

class Location;

class Server {
	private :
		int port;
		std::string root;
		std::string host;
		std::string hostPort;
		std::string realPath;
		std::string upload;
		std::string uploadPath;
		std::string methods;
		std::string autoIndex;
		std::vector<std::string> serverName;
		long long clientMaxBodySize;
		double timeOut;
		std::vector<std::string> index;
		std::vector<Location> locations;
		std::map<int, std::string> errorPages;
		std::map<std::string, std::string> cgiPaths;
		
	public :
		Server();
		~Server();
		
		void setRoot(std::string value);
		void setPort(std::string value);
		void setHost(std::string value);
		void setIndex(std::string value);
		void setUpload(std::string value);
		void setMethods(std::string value);
		void setTimeOut(std::string value);
		void setAutoIndex(std::string value);
		void setServNames(std::string value);
		void setClientMaxBodySize(std::string value);

		void initHostPort();
		void setCgiPath(std::string value);
		void insertCgiPath(std::string line, std::string value);

		void setErrorPages(std::string value);
		void insertErrorPages(std::string value, std::string token);

		int getPort();
		double getTimeOut();
		std::string getRoot();
		std::string getHost();
		std::string getRealPath();
		std::string getUpload();
		std::string getMethods();
		std::string getHostPort();
		std::string getAutoIndex();
		std::string getUploadPath();
		long long getClientMaxBodySize();
		std::vector<std::string> &getIndex();
		std::vector<Location> &getLocation();
		std::vector<std::string> &getServNames();
		std::map<int, std::string> &getErrorPages();
		std::map<std::string, std::string> &getCgiPaths();
		
		std::string getErrorPagesByKey(int key);
		std::string getCgiByKey(std::string key);
		
		void initEmptyData();
		void checkArg();
		// remove
		// std::string getValue(std::string key);
		// void addArg(std::string key, std::string value);
		
		void setLocValue(Location &locat, std::string key, std::string value);
		void addLocat(Location &Locat);
		
		void printArg();
};

#endif