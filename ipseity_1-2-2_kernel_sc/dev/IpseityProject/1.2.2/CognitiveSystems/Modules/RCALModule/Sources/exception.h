#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <iostream>
#include <cstdio>
#include <sstream>
#include <exception>

class MyException : public std::exception
{
public:
    MyException( const char * Msg)
    {
        std::ostringstream oss;
        oss << "Exception " << Msg;
        this->msg = oss.str();
    }
 
    virtual ~MyException() throw()
    {}
 
    virtual const char * what() const throw()
    {
        return this->msg.c_str();
    }
 
private:
    std::string msg;
};

#endif
