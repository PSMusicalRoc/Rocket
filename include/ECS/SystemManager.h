#ifndef _ROC_SYSTEM_MANAGER_H_
#define _ROC_SYSTEM_MANAGER_H_

#include <memory>
#include <unordered_map>

#include "RocErrorManagement/ErrorHandler.h"
#include "Entity.h"
#include "Component.h"
#include "System.h"

class SystemManager
{
public:
	template<typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		const char* typeName = typeid(T).name();

		if (mSystems.find(typeName) != mSystems.end())
        {
            ROC_SetErrorMessage("Attempting to register System twice (RegisterSystem)");
            return nullptr;
        }

		// Create a pointer to the system and return it so it can be used externally
		auto system = std::make_shared<T>();
		mSystems.insert({typeName, system});
		return system;
	}

    template<typename T>
    std::shared_ptr<T> GetSystem()
    {
        const char* typeName = typeid(T).name();

        if (mSystems.find(typeName) == mSystems.end())
        {
            ROC_SetErrorMessage("Requested System is not registered yet (GetSystem)");
            return nullptr;
        }

        return std::static_pointer_cast<T>(mSystems.at(typeName));
    }

	template<typename T>
	bool SetSignature(Signature signature)
	{
		const char* typeName = typeid(T).name();

		if (mSystems.find(typeName) == mSystems.end())
        {
            ROC_SetErrorMessage("Attempted to use System before registering it (SetSignature)");
            return false;
        }

		// Set the signature for this system
		mSignatures.insert({typeName, signature});
        return true;
	}

	void EntityDestroyed(Entity entity)
	{
		// Erase a destroyed entity from all system lists
		// mEntities is a set so no check needed
		for (auto const& pair : mSystems)
		{
			auto const& system = pair.second;

			system->mEntities.erase(entity);
		}
	}

	void EntitySignatureChanged(Entity entity, Signature entitySignature)
	{
		// Notify each system that an entity's signature changed
		for (auto const& pair : mSystems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = mSignatures[type];

			// Entity signature matches system signature - insert into set
			if ((entitySignature & systemSignature) == systemSignature)
			{
				system->mEntities.insert(entity);
			}
			// Entity signature does not match system signature - erase from set
			else
			{
				system->mEntities.erase(entity);
			}
		}
	}

private:
	// Map from system type string pointer to a signature
	std::unordered_map<const char*, Signature> mSignatures{};

	// Map from system type string pointer to a system pointer
	std::unordered_map<const char*, std::shared_ptr<System>> mSystems{};
};

#endif