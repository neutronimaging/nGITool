//
// This file is part of the recon2 library by Anders Kaestner
// (c) 2011 Anders Kaestner
// Distribution is only allowed with the permission of the author.
//
// Revision information
// $Author: kaestner $
// $File$
// $Date: 2011-09-24 19:34:07 +0200 (Sa, 24 Sep 2011) $
// $Rev: 1076 $
// $Id: BackProjectorBase.h 1076 2011-09-24 17:34:07Z kaestner $
//

#ifndef __INTERACTIONBASE_H
#define __INTERACTIONBASE_H

#include "nGIFramework_global.h"
#include <string>

#include <logging/logger.h>

class NGIFRAMEWORKSHARED_EXPORT InteractionBase
{
	kipl::logging::Logger logger;
public:
	InteractionBase(std::string name="InteractorBase");

	virtual void Reset();
	virtual void Abort();
	virtual void Done();
	/// Updates the status of the reconstruction, to be called by the engine and back-projector
	///
	/// Returns true if the process should be aborted.
	virtual bool SetProgress(float progress, std::string msg="");
	virtual bool Aborted();
	virtual bool Finished();

	virtual float CurrentProgress();
	virtual std::string CurrentMessage();

protected:
	bool m_bAbort;
	bool m_bFinished;
	float m_fProgress;
	std::string m_sMessage;
};


#endif
