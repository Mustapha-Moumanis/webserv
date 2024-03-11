/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 14:31:08 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/11 22:50:32 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <algorithm>
#include <sstream>
#include <vector>
#include <sys/stat.h>
#include <stdlib.h>
#include "Location.hpp"
#include "../include/Utils.hpp"

class Server {
	private :		
		// std::vector<std::string> serverName;
		std::string serverName;
		int port;
		std::string host;
		std::string root;
		long long clientMaxBodySize;
		std::map<std::string, std::string> errorPages;
		std::vector<Location> locations;
		std::vector<std::string> index;
		std::string autoIndex;
		
	public :
		Server();
		~Server();
		
		void setRoot(std::string value);
		void setPort(std::string value);
		void setHost(std::string value);
		void setServNames(std::string value);
		void setClientMaxBodySize(std::string value);
		void setErrorPages(std::string value);
		void setAutoIndex(std::string value);
		void setIndex(std::string value);

		std::string getRoot();
		int getPort();
		std::string getHost();
		std::string getServNames();
		long long getClientMaxBodySize();
		std::map<std::string, std::string> getErrorPages();
		std::string getErrorPagesPath(std::string code);
		std::vector<std::string> getIndex();
		std::vector<Location> &getLocation();
		std::string getAutoIndex();
		
		void checkArg();
		
		// remove
		// std::string getValue(std::string key);
		// void addArg(std::string key, std::string value);
		
		void setLocValue(Location &locat, std::string key, std::string value);
		void addLocat(Location &Locat);
		
		bool isDir(std::string path);
		
		void printArg();
};

#endif