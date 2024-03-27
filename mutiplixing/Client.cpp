/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 14:58:53 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/27 03:17:15 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() : status(1), Response("") {
    // fileName = "Data/" + getNewName() + ".txt";
    // fsBody.open(fileName.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
}

Client::~Client() {}

void Client::setServ(Server *serv) {
    request.setServ(*serv);
    this->serv = serv;
}

void Client::setStatus(bool status) {
    this->status = status;
}

void Client::setDoublicateServer(std::vector<Server *> &vec) {
    request.setDoublicateServer(vec);
    this->doublicateServer = vec;
}

// std::string Client::getNewName()
// {
//     std::string name;
//     char c;

//     std::ifstream randomFile("/dev/random", std::ios::binary);
//     if (!randomFile.is_open())
//         return "";
    
//     while (name.length() < 8)
//     {
//         randomFile.read(&c, 1);
//         if (randomFile.fail())
//             return "";
//         if (std::isalpha(c))
//             name += c;
//     }
//     randomFile.close();
//     return name;
// }

bool Client::getStatus() {
    return status;
}

Server *Client::getServ() {
    return serv;
}

Request &Client::getRequest() {
    return request;
}

std::string Client::getResponse() {
    return Response;
}

std::vector<Server *> &Client::getDoublicateServer() {
    return doublicateServer;
}

std::string Client::generateResponse(int Code, std::string Msg, std::string mimeType) {
    std::string resp;
    std::stringstream ss;
    std::string sCode;
    ss << Code;
    ss >> sCode;
    
    resp = "HTTP/1.1 " + sCode + " " + Msg + "\r\n";
    resp += "Content-Type: " + mimeType + "\r\n\r\n";
    if (Code == 204) 
        return resp;
    
    resp += "<!DOCTYPE html>\n<html lang='en'>\n<head>\n<meta charset='UTF-8'>\n<title>";
    resp += sCode + " " + Msg;
    resp += "</title>\n<style>body {font-family: Arial, sans-serif;background-color: #f7f7f7;margin: 0;padding: 0;}";
    resp += ".container {text-align: center;margin-top: 20vh;}h1 {font-size: 5em;color: #333;}h3 {font-size: 2em;color: #666;}</style>";
    resp += "</head>\n<body>\n<div class='container'>\n";
    resp += "<h1>" + sCode + "</h1>\n";
    resp += "<h3> " + Msg + "</h3>\n";
    resp += "</div>\n</body>\n</html>";
    return resp;
}

std::string Client::generateDirResponse(int Code, std::string const Msg, std::string path) {
    std::string resp;
    std::stringstream ss;
    std::string sCode;
	
    ss << Code;
    ss >> sCode;
    struct dirent* directoryEntries;
	DIR* dir = opendir(path.c_str());
    
    resp = "HTTP/1.1 " + sCode + " " + Msg + "\r\n";
    resp += "Content-Type: text/html\r\n\r\n";
    if (Code == 204) 
        return resp;
    
    resp += "<!DOCTYPE html>\n<html lang='en'>\n<head>\n<meta charset='UTF-8'>\n<title>";
    resp += sCode + " " + Msg;
    resp += "</title>\n<style>body {font-family: Arial, sans-serif;background-color: #f7f7f7;margin: 0;padding: 0;}";
    resp += ".container {text-align: center;margin-top: 20vh;}h1 {font-size: 5em;color: #333;}h3 {font-size: 2em;color: #666;}</style>";
    resp += "</head>\n<body>\n<div class='container'>\n";
    if (!dir)
        throw StatusCodeExcept(403);
    while ((directoryEntries = readdir(dir))){
        std::string name = directoryEntries->d_name;
        std::cout << ">> " << name << std::endl;
        
    }
    closedir(dir);
    resp += "</div>\n</body>\n</html>";
    return resp;
}

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
        std::cout << Response << std::endl;
        setStatus(0);
    }
    catch (const responseGetExcept &e){
        if (e.getIsFile())
            std::cout << "is file\n";
        else {
            std::cout << "is folder\n";
            Response = generateDirResponse(e.getStatusCode(), e.what(), e.getURL());
            std::cout << Response << std::endl;
        }
        setStatus(0);
    }
    catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        setStatus(0);
    }
}
