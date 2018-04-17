//
// This file is part of the recon2 library by Anders Kaestner
// (c) 2011 Anders Kaestner
// Distribution is only allowed with the permission of the author.
//
// Revision information
// $Author: kaestner $
// $File$
// $Date: 2011-12-06 17:38:48 +0100 (Tue, 06 Dec 2011) $
// $Rev: 1136 $
// $Id: ModuleItem.h 1136 2011-12-06 16:38:48Z kaestner $
//

#ifndef __MODULEITEM_H
#define __MODULEITEM_H
#include "nGIFramework_global.h"
#include <string>
#include <logging/logger.h>

#include <ModuleItemBase.h>
#include "InteractionBase.h"
#include "PreprocModuleBase.h"
#include "EstimatorBase.h"

#ifdef _MSC_VER // Shared object specific for msvc
typedef void * (__cdecl *BP_FACTORY)(const char *, const char *, void *) ;
#else // Shared object specific for gcc
typedef void * (*BP_FACTORY)(const char *, const char *, void *) ;
#endif


class NGIFRAMEWORKSHARED_EXPORT ModuleItem : public ModuleItemBase
{
public:
	ModuleItem(std::string sharedobject, std::string modulename);

	PreprocModuleBase * GetModule() {return dynamic_cast<PreprocModuleBase *>(ModuleItemBase::GetModule());}



	virtual ~ModuleItem();
private:
};

class NGIFRAMEWORKSHARED_EXPORT EstimatorItem
{
protected:
	kipl::logging::Logger logger;
public:
	EstimatorItem(std::string sharedobject, std::string modulename, InteractionBase *interactor=NULL);
	EstimatorItem(EstimatorItem & item);
	EstimatorItem & operator=(EstimatorItem & item);

	EstimatorBase * GetModule();
	bool Valid() {return m_Module!=NULL;}
	
	virtual ~EstimatorItem();
private:
	void LoadModuleObject(InteractionBase *interactor);
	void Destroy();


	HINSTANCE hinstLib;
    BP_FACTORY m_fnModuleFactory;
	DESTROYER m_fnDestroyer;

	std::string m_sSharedObject;
	std::string  m_sModuleName;

	EstimatorBase *m_Module;

};

#endif
