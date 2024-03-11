/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 14:58:56 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/11 15:32:43 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../parse/Server.hpp"
#include "Request.hpp"
#include "StatusCodeExcept.hpp"
#include <fstream>

class Client {
	private :
		Server *serv;
		bool status;
		Request request;
		// response atribuite
		std::string Response;
		// std::fstream Response;

	public :
		Client();
		Client(Client const &other);
		Client &operator=(Client const &other);
		~Client();
		
		void setServ(Server &serv);
		void setStatus(bool status);
		void SentRequest(std::string);
		
		Server *getServ();
		bool getStatus();
		Request &getRequest();
		std::string getResponse();
		
		std::string generateResponse(HttpStatus::StatusCode Code, std::string const Msg, std::string mimeType);
		
		// std::string getNewName();
};

#endif