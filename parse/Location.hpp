/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:50:20 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/19 02:01:32 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "Server.hpp"

class Server;

class Location {
	private :
		std::string root;
		std::string path;
		std::string upload;
		std::string methods; 
		std::string autoIndex; 
		int rediractionStatusCode;
		std::string rediractionURL;
		
		long long clientMaxBodySize;
		std::vector<std::string> index; 
		std::map<std::string, std::string> errorPages;
		std::map<std::string, std::string> cgiPaths;

	public :
		Location(Server &serv);
		~Location();
		
		void setRoot(std::string value);
		void setPath(std::string value);
		void setIndex(std::string value);
		void setUpload(std::string value);
		void setMethods(std::string value);
		void setAutoIndex(std::string value);
		void setRediraction(std::string value);
		void setClientMaxBodySize(std::string value);

		void setCgiPath(std::string value);
		void insertCgiPath(std::string line, std::string value);

		void setErrorPages(std::string value);
		void insertErrorPages(std::string str, std::string value);

		std::string getRoot();
		std::string getPath();
		std::string getUpload();
		std::string getMethods();
		std::string getAutoIndex();
		// std::string getRediraction();
		std::string getRediractionURL();
		int getRediractionStatusCode();
		long long getClientMaxBodySize();
		std::vector<std::string> &getIndex();
		std::map<std::string, std::string> &getErrorPages();
		std::map<std::string, std::string> &getCgiPaths();

		std::string getCgiByKey(std::string key);
		std::string getErrorPagesPath(std::string key);
		
		void initEmptyData(Server &serv);
		void checkLocation();
		void printArg();
};

#endif