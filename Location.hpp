/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:50:20 by mmoumani          #+#    #+#             */
/*   Updated: 2024/02/23 18:35:02 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <vector>
#include <map>

class Server;

class Location {
	private :
		std::map<std::string, std::string> arg;
		
		std::string root;
		std::string path;
		std::string methods;
	public :
		Location(Server &serv);
		~Location();
		
		void setRoot(std::string value);
		void setPath(std::string value);
		void setmethods(std::string value);
		
		std::string getRoot();
		std::string getPath();
		std::string getmethods();
		
		void addArg(std::string key, std::string value);
		
		void checkLocation();
		void printArg();
};

#endif