#ifndef _ROC_SYSTEM_H_
#define _ROC_SYSTEM_H_

#include <set>
#include "Entity.h"

class System
{
public:
	std::set<Entity> mEntities;
	virtual Signature GetSignature() = 0;
};

#endif