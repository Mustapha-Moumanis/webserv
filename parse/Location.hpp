/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:50:20 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/14 17:53:21 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <map>
#include <vector>
#include <iostream>
#include "Server.hpp"

class Server;

class Location {
	private :
		std::string root;
		std::string path;
		std::string methods;
		std::string autoIndex;
		std::string rediraction;
		std::vector<std::string> index;
		std::map<std::string, std::string> errorPages;
		// std::map<std::string, std::string> cgipath;

	public :
		Location(Server &serv);
		~Location();
		
		void setRoot(std::string value);
		void setPath(std::string value);
		void setIndex(std::string value);
		void setMethods(std::string value);
		void setAutoIndex(std::string value);
		void setErrorPages(std::string value);
		void insertErrorPages(std::string str, std::string value);
		void setRediraction(std::string value);

		std::string getRoot();
		std::string getPath();
		std::string getMethods();
		std::string getAutoIndex();
		std::string getRediraction();
		std::vector<std::string> &getIndex();
		std::map<std::string, std::string> getErrorPages();
		
		std::string getErrorPagesPath(std::string key);
		
		void checkLocation();
		void printArg();
};

#endif