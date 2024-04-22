/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shilal <shilal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 14:36:33 by shilal            #+#    #+#             */
/*   Updated: 2024/04/22 16:29:42 by shilal           ###   ########.fr       */
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
		bool headFlag;
		clock_t *ptrTime;
		bool *ptrIsCgi;
		DIR* dir;

	public :
		Request();
		~Request();
	
		void setServ(Server&);
		Location *getLocation();
		bool getHeadFlag();
		void setRequest(std::string);
		void CheckFirstLine(std::string);
		void CheckRequest(void);
		void matchingURL(std::string b);
		void parseURL(std::string &);
		bool CompareURL(std::string s1, std::string s2);
		void setDoublicateServer(std::vector<Server *> &vec);
		void specificServ(void);
		void checkTimeOut(void);
		void checkCgiTimeOut(void);
		void setPtrTime(clock_t *time);
		void setPtrIsCgi(bool *isCgi);

	private :
		std::map<std::string, std::string> HeaderCgi;
		FILE *ftype;
		FILE *fCgi;
		pid_t p;
		std::string contentType;
		std::string nextchunk;
		std::string fileName;
		std::string type;
		std::string path;
		std::string cookies;
		std::string fileCgiName;
		long long ContentLength;
		long long length;
		bool IsChunked;
		int	buffer;
		bool parentDir;

	public :
		// Get 
		void Get(void);
		// bool hasIndexFile(std::string url); // remove
		void isDirHasIndexFile(void);
		void generateDirAutoIndex(void);
		std::string genGetDirHeader(int code, std::string mimeType);
		std::string genDirItem(std::string name);

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
		void cgiPost(int, std::string);
		void cgiGet(std::string, std::string);
		std::string rediractionCGI(void);
		void parssRspCGI();

		std::string genGetFileHeader(int code, std::string url);
};

#endif