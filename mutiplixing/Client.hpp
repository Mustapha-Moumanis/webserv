/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 14:58:56 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/29 22:30:59 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <fstream>
#include <dirent.h>
#include <cctype>
#include "StatusCodeExcept.hpp"
#include "rediractionExcept.hpp"
#include "responseGetExcept.hpp"
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
		
		std::ifstream ifs;
		std::string header;
		bool isThingsToRes;

	public :
		Client();
		~Client();
		
		void setServ(Server *serv);
		void setStatus(bool status);
		void SentRequest(std::string);
		void setHeader(std::string header);
		void setThingsToRes(bool isThingsToRes);
		void setDoublicateServer(std::vector<Server *> &vec);
		
		
		Server *getServ();
		bool getStatus();
		bool getThingsToRes();
		Request &getRequest();
		std::string getHeader();
		std::string getResponse();
		std::ifstream &getInFileStream();
	
		std::vector<Server *> &getDoublicateServer();
		
		std::string generateHeaderResponse(std::string Code, std::string Msg, std::string mimeType);
		std::string generateResponse(int Code, std::string const Msg, std::string mimeType);
		std::string generateDirResponse(int Code, std::string const Msg, std::string body);
};

#endif