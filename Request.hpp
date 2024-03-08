#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Server.hpp"
#include <fstream>

class Request {
	
    private :
		std::map<std::string, std::string> HeadReq;
		Server *server;
		int HeaderIsDone;
		std::string body;

	public :
		Request();
		~Request();
	
		void setServ(Server&);
		void setRequest(std::string);
		void CheckFirstLine(std::string);
		void CheckRequest();
		bool matchingURL(std::string b);

		void Get(void);
		// Post();
		// Delete();

		void checkUrl(std::string);
};

#endif