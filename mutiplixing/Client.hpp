/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shilal <shilal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 14:58:56 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/25 00:53:41 by shilal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <fstream>
#include <cctype>
#include "StatusCodeExcept.hpp"
#include "rediractionExcept.hpp"
#include "../parse/Server.hpp"
#include "Request.hpp"

class Client {
	private :
		Server *serv;
		std::vector<Server *> doublicateServer;
		
		bool status;
		Request request;
		// response atribuite
		std::string Response;
		// std::fstream Response;

	public :
		Client();
		~Client();
		
		void setServ(Server *serv);
		void setStatus(bool status);
		void SentRequest(std::string);
		void setDoublicateServer(std::vector<Server *> &vec);
		
		Server *getServ();
		bool getStatus();
		Request &getRequest();
		std::string getResponse();
		std::vector<Server *> &getDoublicateServer();
		
		std::string generateResponse(int Code, std::string const Msg, std::string mimeType);
		
		// std::string getNewName();
};

#endif