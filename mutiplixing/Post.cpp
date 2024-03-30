/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shilal <shilal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 00:41:29 by shilal            #+#    #+#             */
/*   Updated: 2024/03/30 00:42:54 by shilal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

int Request::setfirstBody(){
	
	std::string fileName = path + "/" + getNewName() + type;
	ftype = fopen(fileName.c_str(), "w+");
	if (ftype == NULL) 
		throw StatusCodeExcept(403);

	std::string num = body.substr(0, body.find("\r\n"));
	std::stringstream stream;
	stream << num;
	stream >> std::hex >> buffer;
	if (buffer == 0)
		throw StatusCodeExcept(204);

	body = body.substr(body.find("\r\n") + 2);
	this->length = body.length();
	if (this->length > buffer){
		std::stringstream stream;
		stream << body.substr(body.find("\r\n") + 2);
		stream >> std::hex >> buffer;
		if (buffer == 0){
			body = body.substr(0, body.find("\r\n"));
			fwrite (body.c_str() , sizeof(char), body.length(), ftype);
			return (201);
		}
	}
	fwrite (body.c_str() , sizeof(char), body.length(), ftype);
	body.clear();
	return (0);
}

int Request::getBuffer(std::string req){
		
	std::string num = req.substr(0, req.find("\r\n"));
	std::stringstream stream;
	stream << num;
	stream >> std::hex >> buffer;
	if (buffer == 0)
		return (201);

	req = req.substr(req.find("\r\n") + 2);
	fwrite (req.c_str() , sizeof(char), req.length(), ftype);
	this->length = req.length();
	return (0);
}

int Request::postChunked(std::string req){

	if (!body.empty())
		return (setfirstBody());
	else if (!nextchunk.empty()){
		req = nextchunk + req;
		if (req[0] == '\r')
			req = req.substr(req.find("\r\n") + 2);
		nextchunk.clear();
		return (getBuffer(req));
	}
	else {
		size_t chunkBuffer;
		this->length += req.length();
		if (this->length > buffer) {
			chunkBuffer = req.length() - (this->length - buffer); // calcul how much i should add to the chunke.
			std::string cut = req.substr(0, chunkBuffer);
			fwrite (cut.c_str() , sizeof(char), cut.length(), ftype); // Push the missing part of chunke.
			req = req.substr(chunkBuffer); // remove the missing part of chunke from the request.
	
			size_t pos;
			pos = req.find("\r\n");
			if (pos != std::string::npos && pos + 2 < req.length()){
				req = req.substr(pos + 2);
				pos = req.find("\r\n");
				if (pos != std::string::npos && pos + 2 < req.length())
					return (getBuffer(req));
			}
			nextchunk = req; // This when the format (\r\nBUFFER\r\n) not correct.
			req.clear();
		}
		else if (this->length == buffer){
			fwrite (req.c_str() , sizeof(char), req.length(), ftype);
			nextchunk = "";
		}
		else
			fwrite (req.c_str() , sizeof(char), req.length(), ftype);
	}
	return 0;
}

int Request::postBinary(std::string req){

	if (!body.empty()){
		std::string fileName = path + "/" + getNewName() + type;
		ftype = fopen(fileName.c_str(), "w+");
		if (ftype == NULL) 
			throw StatusCodeExcept(403);

		fwrite (body.c_str() , sizeof(char), body.length(), ftype);
		this->length = body.length();
		if (this->length >= ContentLength){
			return (201);
		}
		body.clear();
	}
	else {
		fwrite (req.c_str() , sizeof(char), req.length(), ftype);
		this->length += req.length();
		if (this->length >= ContentLength){
			return (201);
		}
	}
	return 0;
}

void Request::Post(std::string req) {

	int check;
	if (HeadReq.find("Transfer-Encoding") != HeadReq.end())
		check = postChunked(req);
	else
		check = postBinary(req);
	// check if ther's cgi script
	if (check == 201)
		rediractionCGI();
}
