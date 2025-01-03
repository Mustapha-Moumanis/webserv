/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:50:20 by mmoumani          #+#    #+#             */
/*   Updated: 2024/04/04 22:32:13 by mmoumani         ###   ########.fr       */
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
		std::string realPath;
		std::string autoIndex; 
		std::string uploadPath;
		int rediractionStatusCode;
		std::string rediractionURL;
		
		std::vector<std::string> index; 
		std::map<int, std::string> errorPages;
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

		void setCgiPath(std::string value);
		void insertCgiPath(std::string line, std::string value);

		void setErrorPages(std::string value);
		void insertErrorPages(std::string str, std::string value);

		std::string getRoot();
		std::string getPath();
		std::string getUpload();
		std::string getMethods();
		std::string getAutoIndex();
		std::string getUploadPath();
		std::string getRediractionURL();
		int getRediractionStatusCode();
		std::vector<std::string> &getIndex();
		std::map<int, std::string> &getErrorPages();
		std::map<std::string, std::string> &getCgiPaths();

		std::string getErrorPagesByKey(int key);
		std::string getCgiByKey(std::string key);
		
		void initEmptyData(Server &serv);
		void checkLocation();
		void printArg();
};

#endif