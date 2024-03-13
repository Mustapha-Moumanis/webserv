#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <fstream>
#include <dirent.h>
#include <unistd.h>
#include "../parse/Server.hpp"
#include "../include/Utils.hpp"
#include "StatusCodeExcept.hpp"

class Request {
	
    private :
		std::map<std::string, std::string> HeadReq;
		Server *server;
		Location *location;
		int HeaderIsDone;
		std::string body;
		std::string url;
		std::string queryString;

	public :
		Request();
		~Request();
	
		void setServ(Server&);
		void setRequest(std::string);
		void CheckFirstLine(std::string);
		void CheckRequest();
		void matchingURL(std::string b);
		bool CompareURL(std::string s1, std::string s2);
	
		void Get(void);
		void Delete(void);
		void Post(std::string);

		void RemoveContentDir(std::string);
};

#endif