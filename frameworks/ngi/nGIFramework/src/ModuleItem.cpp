//<LICENSE>
#include "stdafx.h"
#include <string>
#include <sstream>
#ifndef _MSC_VER
#include <dlfcn.h>
#endif

#include "../include/ModuleItem.h"
#include "../include/nGIException.h"



//--------------------------------------------------
// Module item
ModuleItem::ModuleItem(std::string sharedobject, std::string modulename):
	ModuleItemBase("ngitool",sharedobject,modulename)
{
}

ModuleItem::~ModuleItem()
{
	logger(kipl::logging::Logger::LogVerbose,"Destroying");
}


///-------------------------------------------------------

EstimatorItem::EstimatorItem(std::string sharedobject, std::string modulename, InteractionBase *interactor)	:
	logger("EstimatorItem"),
	hinstLib(NULL),
	m_fnModuleFactory(NULL),
	m_fnDestroyer(NULL),
	m_Module(NULL)
{
	m_sModuleName=modulename;
	m_sSharedObject=sharedobject;

	LoadModuleObject(interactor);
}

EstimatorItem::EstimatorItem(EstimatorItem & item):
	logger("EstimatorItem")
{
	hinstLib=item.hinstLib;
	m_fnModuleFactory=item.m_fnModuleFactory; 
	m_fnDestroyer=item.m_fnDestroyer;

	m_sSharedObject=item.m_sSharedObject;
	m_sModuleName=item.m_sModuleName;

	m_Module=item.m_Module;
}

EstimatorItem & EstimatorItem::operator=(EstimatorItem & item)
{
	hinstLib=item.hinstLib;
	m_fnModuleFactory=item.m_fnModuleFactory; 
	m_fnDestroyer=item.m_fnDestroyer;

	m_sSharedObject=item.m_sSharedObject;
	m_sModuleName=item.m_sModuleName;

	m_Module=item.m_Module;

	return *this;
}

EstimatorBase * EstimatorItem::GetModule()
{
	return m_Module;
}

	
EstimatorItem::~EstimatorItem()
{
	logger(kipl::logging::Logger::LogVerbose,"D'tor");
	Destroy();
}

void EstimatorItem::LoadModuleObject(InteractionBase *interactor)
{
	std::ostringstream msg;


#ifdef _MSC_VER
	std::wstring so(m_sSharedObject.length(),' ');

	copy(m_sSharedObject.begin(),m_sSharedObject.end(),so.begin());
	hinstLib = LoadLibrary(so.c_str());
#else
	hinstLib = dlopen(m_sSharedObject.c_str(), RTLD_LAZY);
#endif

	if (hinstLib != NULL) 
    { 
#ifdef _MSC_VER
		m_fnModuleFactory = reinterpret_cast<BP_FACTORY>(GetProcAddress(hinstLib, "GetModule"));
#else
		m_fnModuleFactory = reinterpret_cast<BP_FACTORY>(dlsym(hinstLib, "GetModule"));
#endif
		 // If the function address is valid, call the function.
		if (NULL != m_fnModuleFactory) 
        {
			m_Module=reinterpret_cast<EstimatorBase *>(m_fnModuleFactory("ngitool",m_sModuleName.c_str(), reinterpret_cast<void *>(interactor)));
			if (m_Module==NULL) {
				msg.str("");
				msg<<"Failed to create "<<m_sModuleName<<" from "<<m_sSharedObject;
				throw nGIException(msg.str(),__FILE__,__LINE__);
			}
        }
		else
		{
			msg.str("");
			msg<<"Failed to get the factory from "<<m_sSharedObject<<" (Error: "
#ifdef _MSC_VER
					<<GetLastError()<<")";
#else
					<<dlerror()<<")";
#endif

			throw nGIException(msg.str(),__FILE__,__LINE__);
		}


#ifdef _MSC_VER
		m_fnDestroyer = reinterpret_cast<DESTROYER>(GetProcAddress(hinstLib, "Destroy"));
#else
		m_fnDestroyer = reinterpret_cast<DESTROYER>(dlsym(hinstLib, "Destroy"));
#endif

		if (m_fnDestroyer==NULL) {
			msg.str("");
			msg<<"Failed to get the destroyer from "<<m_sSharedObject<<" (Error: "
#ifdef _MSC_VER
					<<GetLastError()<<")";
#else
					<<dlerror()<<")";
#endif
			throw nGIException(msg.str(),__FILE__,__LINE__);
		}
	}
	else {
		msg.str("");
		msg<<"Failed to open object file "<<m_sSharedObject<<" to load module "<<m_sModuleName<<" (Error: "
#ifdef _MSC_VER
					<<GetLastError()<<")";
#else
					<<dlerror()<<")";
#endif

		throw nGIException(msg.str(),__FILE__,__LINE__);
	}
}

void EstimatorItem::Destroy()
{
	logger(kipl::logging::Logger::LogVerbose,"Destroying");
	
	m_fnDestroyer("ngitool",reinterpret_cast<void *>(m_Module));

#ifdef _MSC_VER
	FreeLibrary(hinstLib);
#else
	 dlclose(hinstLib);
#endif
}
