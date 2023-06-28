#ifndef _ROC_COORDINATOR_H_
#define _ROC_COORDINATOR_H_

#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

class Coordinator
{
public:
	static Coordinator* Get()
	{
		if (mCoordinatorPtr == nullptr)
		{
			mCoordinatorPtr = new Coordinator();
			mCoordinatorPtr->Init();
		}
		return mCoordinatorPtr;
	}

	static void DeleteCoordinator()
	{
		if (mCoordinatorPtr != nullptr)
			delete mCoordinatorPtr;
		mCoordinatorPtr = nullptr;
	}

	void Init()
	{
		// Create pointers to each manager
		mComponentManager = std::make_unique<ComponentManager>();
		mEntityManager = std::make_unique<EntityManager>();
		mSystemManager = std::make_unique<SystemManager>();
	}


	// Entity methods
	Entity CreateEntity()
	{
		return mEntityManager->CreateEntity();
	}

	void DestroyEntity(Entity entity)
	{
		mEntityManager->DestroyEntity(entity);

		mComponentManager->EntityDestroyed(entity);

		mSystemManager->EntityDestroyed(entity);
	}


	// Component methods
	template<typename T>
	bool RegisterComponent()
	{
		return mComponentManager->RegisterComponent<T>();
	}

	template<typename T>
    bool AddComponent(Entity entity, T component)
	{
		if (!mComponentManager->AddComponent<T>(entity, component))
        {
            return false;
        }

		auto signature = mEntityManager->GetSignature(entity);
		signature.set(mComponentManager->GetComponentType<T>(), true);
		mEntityManager->SetSignature(entity, signature);

		mSystemManager->EntitySignatureChanged(entity, signature);
        return true;
	}

	template<typename T>
	bool RemoveComponent(Entity entity)
	{
		if (!mComponentManager->RemoveComponent<T>(entity))
        {
            return false;
        }

		auto signature = mEntityManager->GetSignature(entity);
		signature.set(mComponentManager->GetComponentType<T>(), false);
		mEntityManager->SetSignature(entity, signature);

		mSystemManager->EntitySignatureChanged(entity, signature);
        return true;
	}

	template<typename T>
	T& GetComponent(Entity entity)
	{
		return mComponentManager->GetComponent<T>(entity);
	}

	template<typename T>
	ComponentType GetComponentType()
	{
		return mComponentManager->GetComponentType<T>();
	}


	// System methods
	template<typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		return mSystemManager->RegisterSystem<T>();
	}

    template<typename T>
    std::shared_ptr<T> GetSystem()
    {
        return mSystemManager->GetSystem<T>();
    }

	template<typename T>
	bool SetSystemSignature(Signature signature)
	{
		return mSystemManager->SetSignature<T>(signature);
	}

protected:
	static Coordinator* mCoordinatorPtr;

private:
	std::unique_ptr<ComponentManager> mComponentManager;
	std::unique_ptr<EntityManager> mEntityManager;
	std::unique_ptr<SystemManager> mSystemManager;
};

#endif
