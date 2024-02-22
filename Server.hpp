/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 14:31:08 by mmoumani          #+#    #+#             */
/*   Updated: 2024/02/22 18:37:45 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sstream>
#include <vector>
#include <map>

#include "Location.hpp"

class Server {
	private :
		// std::map<std::string, std::string> arg;
		
		std::vector<std::string> serverName;
		std::string port;
		std::string host;
		std::string root;
		std::string clientMaxBodySize;
		
		std::vector<Location> locations;
	public :
		Server();
		~Server();
		
		void setRoot(std::string value);
		void setPort(std::string value);
		void setHost(std::string value);
		void setClientMaxBodySize(std::string value);
		void setServNames(std::string value);
		
		std::string getRoot();
		std::string getPort();
		std::string getHost();
		std::string getClientMaxBodySize();
		std::vector<std::string> getServNames();
		
		void checkArg();
		
		// remove
		std::string getValue(std::string key);
		// void addArg(std::string key, std::string value);
		
		void addLocat(Location &Locat);
		
		
		void printArg();
};

#endif