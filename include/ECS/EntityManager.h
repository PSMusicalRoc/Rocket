#ifndef _ROC_ENT_MANAGER_H_
#define _ROC_ENT_MANAGER_H_

#include <queue>
#include <array>

#include "RocErrorManagement/ErrorHandler.h"

#include "Entity.h"
#include "Component.h"

class EntityManager
{
private:
    std::queue<Entity> mAvailableEntities;
    std::uint32_t mLivingCount = 0;
    std::array<Signature, MAX_ENTITIES> mSignatures;

public:
    EntityManager()
    {
        for (Entity e = 0; e < MAX_ENTITIES; e++)
        {
            mAvailableEntities.push(e);
        }
    }

    /**
     * Creates an Entity with a unique ID. If there are
     * too many entities in existence, returns MAX_ENTITIES.
     * 
     * @returns On Error - MAX_ENTITIES, else the new Entity ID.
    */
    Entity CreateEntity()
    {
        if (mLivingCount >= MAX_ENTITIES)
        {
            ROC_SetErrorMessage("Tried to instantiate an entity past the Entity limit.");
            return MAX_ENTITIES;
        }
        Entity id = mAvailableEntities.front();
        mAvailableEntities.pop();
        mLivingCount++;

        return id;
    }

    /**
     * Takes an entity's ID and destroys the signatures
     * associated with it, as well as adding the Entity's
     * ID back to the ID queue.
     * 
     * @returns false if entity is out of range, true otherwise.
    */
    bool DestroyEntity(Entity entity)
    {
        if (entity >= MAX_ENTITIES)
        {
            ROC_SetErrorMessage("Entity ID supplied to DestroyEntity is out of range.");
            return false;
        }

        mSignatures[entity].reset();

        mAvailableEntities.push(entity);
        mLivingCount--;
        return true;
    }

    /**
     * Sets a particular Entity's Signature
     * 
     * @returns false if entity is out of range, true otherwise
    */
    bool SetSignature(Entity entity, Signature signature)
    {
        if (entity >= MAX_ENTITIES)
        {
            ROC_SetErrorMessage("Entity ID supplied to SetSignature is out of range.");
            return false;
        }

        mSignatures[entity] = signature;
        return true;
    }

    /**
     * Sets a particular Entity's Signature
     * 
     * @returns NULL if entity is out of range, the Signature otherwise
    */
    Signature GetSignature(Entity entity)
    {
        if (entity >= MAX_ENTITIES)
        {
            ROC_SetErrorMessage("Entity ID supplied to GetSignature is out of range.");
            return NULL;
        }

        return mSignatures[entity];
    }
};

#endif