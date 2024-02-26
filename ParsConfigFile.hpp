/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsConfigFile.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 14:15:19 by mmoumani          #+#    #+#             */
/*   Updated: 2024/02/25 19:16:07 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PARSCONFIGFILE_HPP
#define PARSCONFIGFILE_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <sys/stat.h>
#include <sstream>

#include "Server.hpp"

class ParsConfigFile {
	private :
		std::vector<Server> servers;
		std::ifstream ifs;
		std::string line;
		std::string key;
		std::string value;
		size_t AutoSpaces;
		size_t spaces;
		size_t pos;
	public :
		ParsConfigFile(std::string fileName);
		~ParsConfigFile();

		bool isRegFile(std::string path);
		void getKeyValue(std::string line);
		void setServValue(Server &serv, std::string key, std::string value);
		void newServer();
		void newLocation(Server &serv);
};

#endif