//<LICENSE>

#include "stdafx.h"
#include "../include/nGIException.h"
#include <sstream>

nGIException::~nGIException()
{
}

nGIException::nGIException(std::string msg) : kipl::base::KiplException(msg,"nGIException")
{}

nGIException::nGIException(std::string msg, std::string filename, size_t line) :
        kipl::base::KiplException(msg,filename,line,"nGIException")
{}


