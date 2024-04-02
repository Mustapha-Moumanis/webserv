/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shilal <shilal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 14:58:53 by mmoumani          #+#    #+#             */
/*   Updated: 2024/04/02 01:22:45 by shilal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() : status(1), Response(""), header(""), isThingsToRes(0) {
    // fileName = "Data/" + getNewName() + ".txt";
    // fsBody.open(fileName.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
}

Client::~Client() {
    if (ifs.is_open())
        ifs.close();
}

void Client::setServ(Server *serv) {
    request.setServ(*serv);
    this->serv = serv;
}

void Client::setStatus(bool status) {
    this->status = status;
}

void Client::setHeader(std::string header) {
    this->header = header;
}

void Client::setThingsToRes(bool isThingsToRes) {
    this->isThingsToRes = isThingsToRes;
}

void Client::setDoublicateServer(std::vector<Server *> &vec) {
    request.setDoublicateServer(vec);
    this->doublicateServer = vec;
}

bool Client::getStatus() {
    return status;
}

bool Client::getThingsToRes() {
    return isThingsToRes;
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

std::vector<Server *> &Client::getDoublicateServer() {
    return doublicateServer;
}

void Client::responseFile(std::string header, std::string path, size_t pos) {
    ifs.open(path.c_str(), std::ios::binary);
    ifs.seekg(pos, std::ios_base::beg);
    
    this->header = header;
    isThingsToRes = 1;
}

std::string Client::generateHeaderResponse(std::string Code, std::string Msg, std::string mimeType) {
    std::string header;
    header = "HTTP/1.1 " + Code + " " + Msg + "\r\n";
    header += "Content-Type: " + mimeType + "\r\n\r\n";
    return header;
}

std::string Client::generateResponse(int Code, std::string Msg, std::string mimeType) {
    std::string resp;
    std::stringstream ss;
    std::string sCode;
    std::string errorPath;
    Location *location = request.getLocation();
    ss << Code;
    ss >> sCode;
    
    resp = generateHeaderResponse(sCode, Msg, mimeType);
    if (Code == 204) 
        return resp;
    if (isError(Code)) {
        if (location) {
            errorPath = location->getErrorPagesByKey(Code);
            if (!errorPath.empty()){
                responseFile(resp, errorPath, 0);
                return "";
            }
        }
        else {
            errorPath = serv->getErrorPagesByKey(Code);
            if (!errorPath.empty()){
                responseFile(resp, errorPath, 0);
                return "";
            }
        }
    }
        
    resp += "<!DOCTYPE html>\n<html lang='en'>\n<head>\n<meta charset='UTF-8'>\n<title>";
    resp += sCode + " " + Msg;
    resp += "</title>\n<style>body {font-family: Arial, sans-serif;background-color: #f7f7f7;margin: 0;padding: 0;}";
    resp += ".container {text-align: center;margin-top: 20vh;}h1 {font-size: 5em;color: #333;}h3 {font-size: 2em;color: #666;}</style>";
    resp += "</head>\n<body>\n<div class='container'>\n";
    resp += "<h1>" + sCode + "</h1>\n";
    resp += "<h3>" + Msg + "</h3>\n";
    resp += "</div>\n</body>\n</html>";
    return resp;
}

// std::string Client::generateDirResponse(int Code, std::string const Msg, std::string body) {
//     std::string resp;
//     std::stringstream ss;
//     std::string sCode;
//     ss << Code;
//     ss >> sCode;
    
//     resp = generateHeaderResponse(sCode, Msg, "text/html");
//     resp += body;
//     return resp;
// }

void Client::SentRequest(std::string tmp){
    try {
        request.setRequest(tmp);
    }
    catch (const StatusCodeExcept &e) {
        Response = generateResponse(e.getStatusCode(), e.what(), "text/html");
        // std::cout << Response << std::endl;
        setStatus(0);
    }
    catch (const rediractionExcept &e){
        std::string resp;
        std::stringstream ss;
        std::string sCode;

        ss << e.getStatusCode();
        ss >> sCode;
        Response += "HTTP/1.1 " + sCode;
        Response += " ";
        Response += e.what();
        Response += "\r\nLocation: " + e.getURL() + "\r\n";
        Response += "Content-Type: text/html\r\n\r\n";
        setStatus(0);
    }
    catch (const responseGetExcept &e){
        if (e.getIsFile()) {
            responseFile(e.getHeader(), e.getStock(), e.getPos());
            // isThingsToRes = 1;
        }
        else {
            std::cout << "is folder\n";
            Response = e.getHeader() + e.getStock();
        }
        setStatus(0);
    }
}
