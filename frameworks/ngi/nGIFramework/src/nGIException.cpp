/*
 * nGIException.cpp
 *
 *  Created on: Mar 11, 2010
 *      Author: anders
 */
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

std::string nGIException::what()
{
	if (sFileName.empty())
		return sMessage;
	else {
		std::stringstream str;
		str<<"nGI exception in "<<sFileName<<" on line "<<nLineNumber<<": \n"<<sMessage;
		return str.str();
	}
}
