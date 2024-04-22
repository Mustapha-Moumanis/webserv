/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shilal <shilal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 16:38:38 by mmoumani          #+#    #+#             */
/*   Updated: 2024/04/22 16:58:23 by shilal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "../parse/ParsConfigFile.hpp"
#include "Client.hpp"
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <cstdlib>
#include <map>
#include <csignal>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

#define MAX_EVENTS 64

class Webserv {
	private :
		std::vector<Server *> dataServers;
		std::map<std::string, std::vector<Server *> > doublicateServer;
		std::map<std::string, Server *> defaultServer;
		std::map<int, Client *> Clients;
		std::vector<int> fds; // close
		std::map<int, int> indexFD;
		std::string response;

	public :
		Webserv(std::ifstream &ifs);
		~Webserv();
		
		void multiplixing();
		void initDoublicateServer();
		void initDefaultServer();
		std::vector<Server *> &getDoublicateServer(std::string host);
		Server *getDefaultServer(std::string host);

		void requestPart(int fd);
		bool responseFile(int fd);
		bool responseFolder(int fd);
		
};

#endif