//<LICENSE>

#ifndef INTERACTIONBASE_H
#define INTERACTIONBASE_H

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
