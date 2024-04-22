/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shilal <shilal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 14:58:56 by mmoumani          #+#    #+#             */
/*   Updated: 2024/04/22 16:57:26 by shilal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <fstream>
#include <dirent.h>
#include <cctype>
#include <ctime>
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
		std::string header;
		int isThingsToRes;
		
		// file response
		std::ifstream ifs;

		// folder response
		DIR* dir;

		// Timeout
		clock_t time;
		bool ifTimeOut;
		bool isCgi;

	public :
		Client();
		~Client();
		
		void setServ(Server *serv);
		void setStatus(bool status);
		void setIfTimeOut(bool ifTimeOut);
		void setTime(clock_t time);
		void SentRequest(std::string);
		void setHeader(std::string header);
		void setThingsToRes(int isThingsToRes);
		void setDoublicateServer(std::vector<Server *> &vec);
		
		
		Server *getServ();
		bool getStatus();
		clock_t getTime();
		int getThingsToRes();
		bool getIfTimeOut();
		bool getIsCgi();
		Request &getRequest();
		std::string getHeader();
		std::string getResponse();
		std::ifstream &getInFileStream();
		DIR *getDirPtr();
	
		std::vector<Server *> &getDoublicateServer();
		
		void genStatusCodeResp(int Code, std::string const Msg, std::string mimeType);
		void genRediractionResp(int Code, std::string Msg, std::string path, std::string mimeType);

		void responseFile(std::string header, std::string path, size_t pos);
		void responseFolder(std::string header, std::string path);

		void checkSimpleTimeOut(void);
		void checkTimeOutOfCgi(void);
};

#endif