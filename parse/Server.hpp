/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 14:31:08 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/12 22:24:56 by mmoumani         ###   ########.fr       */
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

class Location;

class Server {
	private :		
		// std::vector<std::string> serverName;
		int port;
		std::string root;
		std::string host;
		std::string autoIndex;
		std::string serverName;
		long long clientMaxBodySize;
		std::vector<std::string> index;
		std::vector<Location> locations;
		std::map<std::string, std::string> errorPages;
		
	public :
		Server();
		~Server();
		
		void setRoot(std::string value);
		void setPort(std::string value);
		void setHost(std::string value);
		void setIndex(std::string value);
		void setAutoIndex(std::string value);
		void setServNames(std::string value);
		void setErrorPages(std::string value);
		void setClientMaxBodySize(std::string value);

		int getPort();
		std::string getRoot();
		std::string getHost();
		std::string getServNames();
		std::string getAutoIndex();
		long long getClientMaxBodySize();
		std::vector<std::string> &getIndex();
		std::vector<Location> &getLocation();
		std::map<std::string, std::string> &getErrorPages();
		
		void checkArg();
		// helps u :
		std::string getErrorPagesPath(std::string key);
		
		// remove
		// std::string getValue(std::string key);
		// void addArg(std::string key, std::string value);
		
		void setLocValue(Location &locat, std::string key, std::string value);
		void addLocat(Location &Locat);
		
		void printArg();
};

#endif