#ifndef STATUSCODEEXCEPT_HPP
#define STATUSCODEEXCEPT_HPP

#include <iostream>
#include <exception>
#include "StatusCode.hpp"

class StatusCodeExcept: public std::exception {
    HttpStatus::StatusCode statusCode;
    
    public :
        StatusCodeExcept(HttpStatus::StatusCode code) : statusCode(code){
            std::cout << "this is status code >> " << code << "\n";
        }
        virtual ~StatusCodeExcept() throw() { return ; }
        const char* what() const throw() {
            return HttpStatus::reasonPhrase(this->statusCode);
        }

};

#endif