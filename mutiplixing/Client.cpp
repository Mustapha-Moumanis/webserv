/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shilal <shilal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 14:58:53 by mmoumani          #+#    #+#             */
/*   Updated: 2024/04/03 03:15:24 by shilal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() : status(1), Response(""), header(""), isThingsToRes(0), ifTimeOut(0) {
    time = clock();
    std::cout << "time 1 : " << time << std::endl;
    dir = NULL;
    request.setPtrTime(&time);
}

Client::~Client() {
    if (ifs.is_open())
        ifs.close();
    if (dir != NULL)
        closedir(dir);
}

void Client::setServ(Server *serv) {
    request.setServ(*serv);
    this->serv = serv;
}

void Client::setStatus(bool status) {
    this->status = status;
}

void Client::setIfTimeOut(bool t) {
    this->ifTimeOut = t;
}

void Client::setTime(clock_t time) {
    this->time = time;
}

void Client::setHeader(std::string header) {
    this->header = header;
}

void Client::setThingsToRes(int isThingsToRes) {
    this->isThingsToRes = isThingsToRes;
}

void Client::setDoublicateServer(std::vector<Server *> &vec) {
    request.setDoublicateServer(vec);
    this->doublicateServer = vec;
}

bool Client::getStatus() {
    return status;
}

clock_t Client::getTime(){
    return time;
}

int Client::getThingsToRes() {
    return isThingsToRes;
}

bool Client::getIfTimeOut() {
    return ifTimeOut;
}

Server *Client::getServ() {
    return serv;
}

std::string Client::getHeader() {
    return header;
}

Request &Client::getRequest() {
    return request;
}

std::string Client::getResponse() {
    return Response;
}

std::ifstream &Client::getInFileStream() {
    return ifs;
}

DIR *Client::getDirPtr() {
    return dir;
}

std::vector<Server *> &Client::getDoublicateServer() {
    return doublicateServer;
}

void Client::responseFile(std::string header, std::string path, size_t pos) {
    ifs.open(path.c_str(), std::ios::binary);
    ifs.seekg(pos, std::ios_base::beg);
    
    this->header = header;
    isThingsToRes = _FILE;
}

void Client::responseFolder(std::string header, std::string path) {
	dir = opendir(path.c_str());

    this->header = header;
    isThingsToRes = _FOLDER;
}

void Client::genStatusCodeResp(int Code, std::string Msg, std::string mimeType) {
    std::stringstream ss;
    std::string sCode;
    std::string errorPath;
    Location *location = request.getLocation();
    ss << Code;
    ss >> sCode;
    
    Response = "HTTP/1.1 " + sCode + " " + Msg + "\r\n";
    Response += "Content-Type: " + mimeType + "\r\n\r\n";
    
    if (Code == 204 || (Code == 501 && request.getHeadFlag() == 1)) 
        return ;
    if (isError(Code)) {
        if (location) {
            errorPath = location->getErrorPagesByKey(Code);
            if (!errorPath.empty())
                responseFile(Response, errorPath, 0);
        }
        else {
            errorPath = serv->getErrorPagesByKey(Code);
            if (!errorPath.empty())
                responseFile(Response, errorPath, 0);
        }
    }
        
    Response += "<!DOCTYPE html>\n<html lang='en'>\n<head>\n<meta charset='UTF-8'>\n<title>";
    Response += sCode + " " + Msg;
    Response += "</title>\n<style>body {font-family: Arial, sans-serif;background-color: #f7f7f7;margin: 0;padding: 0;}";
    Response += ".container {text-align: center;margin-top: 20vh;}h1 {font-size: 5em;color: #333;}h3 {font-size: 2em;color: #666;}</style>";
    Response += "</head>\n<body>\n<div class='container'>\n";
    Response += "<h1>" + sCode + "</h1>\n";
    Response += "<h3>" + Msg + "</h3>\n";
    Response += "</div>\n</body>\n</html>";
}

void Client::genRediractionResp(int Code, std::string Msg, std::string path, std::string mimeType) {
    std::stringstream ss;
    std::string sCode;

    ss << Code;
    ss >> sCode;
    Response = "HTTP/1.1 " + sCode + " " + Msg;
    Response += "\r\nLocation: " + path + "\r\n";
    Response += "Content-Type: " + mimeType + "\r\n\r\n";
}

void Client::SentRequest(std::string tmp){
    try {
        if (ifTimeOut == 1){
           request.checkTimeOut();
        }
        request.setRequest(tmp);
    }
    catch (const StatusCodeExcept &e) {
        genStatusCodeResp(e.getStatusCode(), e.what(), "text/html");
        setStatus(0);
    }
    catch (const rediractionExcept &e){
        genRediractionResp(e.getStatusCode(), e.what(), e.getURL(), "text/html");
        setStatus(0);
    }
    catch (const responseGetExcept &e){
        if (e.getIsFile() == _FILE)
            responseFile(e.getHeader(), e.getStock(), e.getPos());
        else
            responseFolder(e.getHeader(), e.getStock());
        setStatus(0);
    }
}
