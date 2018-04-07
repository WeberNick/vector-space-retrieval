/**
 *	@file 	exception.hh
 *	@author	Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *	@brief	Provides a central class for exception handling
 *	@bugs 	Currently no bugs known
 *	@todos	-
 *
 *	@section DESCRIPTION
 *	TODO
 *  @section USE
 *	    throw BaseException(__FILE__, __LINE__, __PRETTY_FUNCTION__, "Error Message");
 */
#pragma once

#include <exception>
#include <string>
#include <iostream>
#include <errno.h>

class BaseException : public std::exception
{
    public:
        BaseException(
                const char*         aFileName, 
                const unsigned int  aLineNumber, 
                const char*         aFunctionName, 
                const std::string   aErrorMessage); 
        BaseException(const BaseException& aException);
        BaseException& operator=(const BaseException& aException);
        BaseException(BaseException&&) = delete;
        BaseException& operator=(BaseException&&) = delete;
        ~BaseException();

    public:
        virtual const char* what() const throw() { return _errMsg.c_str(); }
        void print() const;

    private:
        const std::string   _file;
        const unsigned int  _line;
        const std::string   _func;
        const std::string   _errMsg;

};

class OutOfMemoryException : public BaseException
{
	public:
		OutOfMemoryException(
                const char*         aFileName, 
                const unsigned int  aLineNumber, 
                const char*         aFunctionName);
};

class InvalidArguementException : public BaseException
{
	public:
		InvalidArguementException(
                const char*         aFileName, 
                const unsigned int  aLineNumber, 
                const char*         aFunctionName, 
                const std::string   aErrorMessage);
};

class SwitchException : public BaseException
{
	public:
		SwitchException(const char*  aFileName, const unsigned int aLineNumber, const char* aFunctionName);
};

class FileException : public BaseException
{
	public:
		FileException(
                const char*         aFileName, 
                const unsigned int  aLineNumber, 
                const char*         aFunctionName, 
                const char*         aErrorFileName, 
                const std::string   aErrorMessage); 
};
