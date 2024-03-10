/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shilal <shilal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:50:20 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/10 11:21:04 by shilal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
// #include <vector>
#include <map>

// #include <fstream>
// #include <vector>
// #include <map>
// #include <sys/stat.h>
// #include <sstream>

class Server;

class Location {
	private :
		std::string root;
		std::string path;
		std::string rediraction;
		std::string autoIndex;
		std::string methods;
		std::map<std::string, std::string> cgipath;

	public :
		Location(Server &serv);
		~Location();
		
		void setRoot(std::string value);
		void setPath(std::string value);
		void setmethods(std::string value);
		void setRediraction(std::string value);
		void setAutoIndex(std::string value);

		std::string getRoot();
		std::string getPath();
		std::string getmethods();
		std::string getRediraction();
		std::string getAutoIndex();
		
		void checkLocation();
		void printArg();
};

#endif