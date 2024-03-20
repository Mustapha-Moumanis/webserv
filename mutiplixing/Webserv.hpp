/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 16:38:38 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/20 00:36:32 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "../parse/ParsConfigFile.hpp"
#include "Client.hpp"
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <stdlib.h>

#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

#define MAX_EVENTS 64

class Webserv {
	private :
		std::vector<Server *> dataServers;
		std::map<std::string, std::vector<Server *> > doublicateServer;
		std::map<int, Client *> Clients;
		std::vector<int> fds; // close
		std::map<int, int> indexFD;

	public :
		Webserv(std::ifstream &ifs);
		~Webserv();
		
		void multiplixing();
		void initDoublicateServer();
		std::vector<Server *> &getDoublicateServer(std::string host);
};

#endif