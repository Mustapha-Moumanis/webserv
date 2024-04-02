/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 14:36:33 by shilal            #+#    #+#             */
/*   Updated: 2024/04/01 23:57:15 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <fstream>
#include <dirent.h>
#include <sys/wait.h>
#include "../parse/Server.hpp"
#include "../include/Utils.hpp"
#include "../include/MimeTypes.hpp"
#include "StatusCodeExcept.hpp"
#include "rediractionExcept.hpp"
#include "responseGetExcept.hpp"

class Request {
	
    private :
		std::map<std::string, std::string> HeadReq;
		Server *server;
		std::vector<Server *> doublicateServer;
		Location *location;
		std::string body;
		std::string queryString;
		std::string reqURL;
		std::string url;
		std::string Method;
		int HeaderIsDone;

	public :
		Request();
		~Request();
	
		void setServ(Server&);
		Location *getLocation();
		void setRequest(std::string);
		void CheckFirstLine(std::string);
		void CheckRequest(void);
		void matchingURL(std::string b);
		void parseURL(std::string &);
		bool CompareURL(std::string s1, std::string s2);
		void setDoublicateServer(std::vector<Server *> &vec);
		void specificServ(void);
		
	private :
		FILE* ftype;
		std::string nextchunk;
		std::string type;
		std::string path;
		std::string fileName;
		long long ContentLength;
		long long length;
		bool IsChunked;
		int	buffer;

	public :
		// Get 
		void Get(void);
		bool hasIndexFile(std::string url); // remove
		void isDirHasIndexFile(void);
		void generateDirAutoIndex(void);
		std::string genGetDirHeader(int code, std::string mimeType);

		// Post
		void Post(std::string);
		int postChunked(std::string);
		int postBinary(std::string);
		int setfirstBody(void);
		int getBuffer(std::string);

		// Delete
		void Delete(void);
		void RemoveContentDir(std::string);

		// CGI
		void cgitest(int, std::string, std::string);
		void rediractionCGI(void);
		void parssRspCGI(FILE*);
		std::string genGetFileHeader(int code, std::string url);
};

#endif