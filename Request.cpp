#include "Request.hpp"
#include <fstream>
#include <cctype>

Request::Request() : HeaderIsDone(0), body("") {}

Request::~Request() {}

void Request::CheckFirstLine(std::string Fline){

	std::stringstream ss(Fline);

	std::string version;
	std::string a;
	std::string b;
	ss >> a >> b >> version;
	if (a != "GET" && a != "DELETE" && a != "POST")
		throw::std::runtime_error("Error Methode not emplemented");
	else
		HeadReq.insert(std::pair<std::string,std::string>("Methode", a));
    
    // this is when u can parss uri 
    if (b.length() > 2048)
		throw::std::runtime_error("414 URI Too Long");
    else
	    HeadReq.insert(std::pair<std::string,std::string>("Location",b));
    
	if (version != "HTTP/1.1")
		throw("Error in HTTP/1.1");
}

void Request::setRequest(std::string req) {

    if (HeaderIsDone == 0){
		CheckFirstLine(req.substr(0, req.find("\r\n")));
		req.erase(0, req.find("\r\n") + 2);
		while (404){
			std::string key = req.substr(0, req.find(": "));
			if (req.substr(0, req.find("\r\n")) == ""){
				req.erase(0, req.find("\r\n") + 2);
				break;
			}
			req.erase(0, req.find(": ") + 2);
			std::string val =  req.substr(0, req.find("\r\n"));
			HeadReq.insert(std::pair<std::string,std::string>(key,val));
			req.erase(0, req.find("\r\n") + 2);
		}
        body = req;
        HeaderIsDone = 1;
        CheckRequest();
	}
	else if (HeaderIsDone == 1 && HeadReq.find("Methode")->second == "POST")
        body += req;
    else
        return;
}

void Request::CheckRequest(){
	std::map<std::string, std::string>::iterator it;
	if (HeadReq.find("Methode")->second == "POST"){
		if ((it = HeadReq.find("Content-Length")) != HeadReq.end())
			std::cout << "Not chencked" << std::endl;
		else if ((it = HeadReq.find("Transfer-Encoding")) != HeadReq.end()){
			if (it->second != "chunked")
				throw::std::runtime_error("501 Not Implemanted");	
		}
		else
			throw::std::runtime_error("400 Bad request");
		std::cout << "Thers POST methode" << std::endl;
	}
}

void Request::setServ(Server &serv) {
	this->server = &serv;
    std::cout << "request serv port" << this->server->getPort() << std::endl;

}
