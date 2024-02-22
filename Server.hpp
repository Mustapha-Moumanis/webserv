/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 14:31:08 by mmoumani          #+#    #+#             */
/*   Updated: 2024/02/22 14:05:00 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <map>

#include "Location.hpp"

class Server {
	private :
		std::map<std::string, std::string> arg;
		std::vector<Location> locations;
	public :
		Server();
		~Server();
		
		std::string getValue(std::string key);
		void addArg(std::string key, std::string value);
		void addLocat(Location &Locat);
		void printArg();
};

#endif