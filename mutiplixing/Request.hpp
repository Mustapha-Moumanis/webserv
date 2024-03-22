/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shilal <shilal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 14:36:33 by shilal            #+#    #+#             */
/*   Updated: 2024/03/22 20:41:25 by shilal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <fstream>
#include <dirent.h>
#include <unistd.h>
#include "../parse/Server.hpp"
#include "../include/Utils.hpp"
#include "../include/MimeTypes.hpp"
#include "StatusCodeExcept.hpp"

class Request {
	
    private :
		std::map<std::string, std::string> HeadReq;
		Server *server;
		std::vector<Server *> doublicateServer;
		Location *location;
		std::string body;
		std::string queryString;
		std::string url;
		std::string Method;
		int HeaderIsDone;

	public :
		Request();
		~Request();
	
		void setServ(Server&);
		void setRequest(std::string);
		void CheckFirstLine(std::string);
		void CheckRequest();
		void matchingURL(std::string b);
		bool CompareURL(std::string s1, std::string s2);
		void setDoublicateServer(std::vector<Server *> &vec);
		void specificServ();

	
	private :
		std::ofstream ftype;
		std::string nextchunk;
		std::string type;
		long long ContentLength;
		long long length;
		bool IsChunked;
		int	buffer;

	public :
		// Get 
		void Get(void);
		
		// Post
		void Post(std::string);
		void PostChunked(std::string);
		void setfirstBody();
		void getBuffer(std::string);

		// Delete
		void Delete(void);
		void RemoveContentDir(std::string);
};

#endif