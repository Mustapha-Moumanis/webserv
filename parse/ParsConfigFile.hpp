/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsConfigFile.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 14:15:19 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/18 18:15:58 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PARSCONFIGFILE_HPP
#define PARSCONFIGFILE_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "Server.hpp"
#include "../include/Utils.hpp"

class ParsConfigFile {
	private :
		std::vector<Server *> &dataServers;
		std::ifstream &ifs;
		std::string line;
		std::string key;
		std::string value;
		size_t AutoSpaces;
		size_t spaces;
		size_t pos;

	public :
		ParsConfigFile(std::ifstream &ifs, std::vector<Server *> &serv);
		~ParsConfigFile();

		void getKeyValue(std::string line);
		void setServValue(Server &serv, std::string key, std::string value);
		void newServer();
		void newLocation(Server &serv);
};

#endif