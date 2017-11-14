/*
 * DpcException.h
 *
 *  Created on: Mar 11, 2010
 *      Author: anders
 */

#ifndef NGIEXCEPTION_H_
#define NGIEXCEPTION_H_

#include "nGIFramework_global.h"
#include <string>
#include <base/KiplException.h>

class NGIFRAMEWORKSHARED_EXPORT nGIException : public kipl::base::KiplException
{
public:
	nGIException(std::string msg);
	nGIException(std::string msg, std::string filename, size_t line);
	virtual ~nGIException();

	virtual std::string what();
};

#endif /* NGIEXCEPTION_H_ */

