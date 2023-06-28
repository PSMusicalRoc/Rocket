#ifndef _ROC_COMPONENT_MANAGER_H_
#define _ROC_COMPONENT_MANAGER_H_

#include <memory>

#include "IComponentArray.h"

class ComponentManager
{
public:
	template<typename T>
	bool RegisterComponent()
	{
		const char* typeName = typeid(T).name();

		if (mComponentTypes.find(typeName) != mComponentTypes.end())
        {
            ROC_SetErrorMessage("Attempting to register ComponentType twice. (RegisterComponent)");
            return false;
        }

		// Add this component type to the component type map
		mComponentTypes.insert({typeName, mNextComponentType});

		// Create a ComponentArray pointer and add it to the component arrays map
		mComponentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

		// Increment the value so that the next component registered will be different
		++mNextComponentType;
        return true;
	}

	template<typename T>
	ComponentType GetComponentType()
	{
		const char* typeName = typeid(T).name();

		if (mComponentTypes.find(typeName) == mComponentTypes.end())
        {
            ROC_SetErrorMessage("Attempted to access ComponentType before registering (GetComponentType)");
            return MAX_COMPONENTS;
        }

		// Return this component's type - used for creating signatures
		return mComponentTypes[typeName];
	}

	template<typename T>
	bool AddComponent(Entity entity, T component)
	{
		// Add a component to the array for an entity
        std::shared_ptr<ComponentArray<T>> ptr = GetComponentArray<T>();
        if (ptr == nullptr) return false;
		return ptr->InsertData(entity, component);
	}

	template<typename T>
	bool RemoveComponent(Entity entity)
	{
		// Remove a component from the array for an entity
        std::shared_ptr<ComponentArray<T>> ptr = GetComponentArray<T>();
        if (ptr == nullptr) return false;
		return ptr->RemoveData(entity);
	}

	template<typename T>
	T& GetComponent(Entity entity)
	{
		// Get a reference to a component from the array for an entity
        std::shared_ptr<ComponentArray<T>> ptr = GetComponentArray<T>();
        /** @todo AGAIN PLEASE FIX, I HATE ASSERTS IN REAL CODE */
        if (ptr == nullptr) assert(false && "GAH");
		return ptr->GetData(entity);
	}

	void EntityDestroyed(Entity entity)
	{
		// Notify each component array that an entity has been destroyed
		// If it has a component for that entity, it will remove it
		for (auto const& pair : mComponentArrays)
		{
			auto const& component = pair.second;

			component->EntityDestroyed(entity);
		}
	}

private:
	// Map from type string pointer to a component type
	std::unordered_map<const char*, ComponentType> mComponentTypes{};

	// Map from type string pointer to a component array
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> mComponentArrays{};

	// The component type to be assigned to the next registered component - starting at 0
	ComponentType mNextComponentType{};

	// Convenience function to get the statically casted pointer to the ComponentArray of type T.
	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray()
	{
		const char* typeName = typeid(T).name();

		if (mComponentTypes.find(typeName) == mComponentTypes.end())
        {
            ROC_SetErrorMessage("Attempted to access nonexistent ComponentArray. (GetComponentArray)");
            return nullptr;
        }

		return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
	}
};

#endif