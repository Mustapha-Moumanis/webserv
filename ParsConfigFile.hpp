/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsConfigFile.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 14:15:19 by mmoumani          #+#    #+#             */
/*   Updated: 2024/02/21 22:18:39 by mmoumani         ###   ########.fr       */
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
		std::ifstream ifs;
		std::vector<Server> servers;
		std::string line;
		std::string key;
		std::string value;
		size_t spaces;
		size_t pos;
	public :
		ParsConfigFile(std::string fileName);
		~ParsConfigFile();

		bool isRegFile(std::string path);
		bool checkExtention(std::string path);
		void getKeyValue(std::string line);
		void newServer();
};

#endif