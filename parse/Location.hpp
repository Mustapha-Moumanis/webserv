/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:50:20 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/12 17:48:45 by mmoumani         ###   ########.fr       */
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
		std::string rediraction;
		std::string methods;
		std::map<std::string, std::string> errorPages;
		std::vector<std::string> index;
		std::string autoIndex;
		// std::map<std::string, std::string> cgipath;

	public :
		Location(Server &serv);
		~Location();
		
		void setRoot(std::string value);
		void setPath(std::string value);
		void setmethods(std::string value);
		void setRediraction(std::string value);
		void setErrorPages(std::string value);
		void setAutoIndex(std::string value);
		void setIndex(std::string value);

		std::string getRoot();
		std::string getPath();
		std::string getmethods();
		std::string getRediraction();
		std::map<std::string, std::string> getErrorPages();
		std::string getErrorPagesPath(std::string code);
		std::string getAutoIndex();
		std::vector<std::string> getIndex();
		
		void checkLocation();
		void printArg();
};

#endif