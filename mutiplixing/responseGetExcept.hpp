#ifndef RESPONSEGETEXCEPT_HPP
#define RESPONSEGETEXCEPT_HPP

#include <iostream>
#include <exception>
#include "../include/Utils.hpp"

class responseGetExcept: public std::exception {
    int statusCode;
    std::string url;
    bool isFile;
    // std::string filePath;
    
    public :
        responseGetExcept(int code, std::string url, bool isFile) : statusCode(code), url(url), isFile(isFile) {}
        virtual ~responseGetExcept() throw() { return ; }
        
        int getStatusCode() const {
            return statusCode;
        };
        
        std::string getURL() const {
            return url;
        };

        bool getIsFile() const {
            return isFile;
        };
        
        const char* what() const throw() {
            return reasonPhrase(this->statusCode);
        }

};

#endif