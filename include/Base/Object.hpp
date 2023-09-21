#ifndef _ROC_OBJECT_INTERFACE_HPP_
#define _ROC_OBJECT_INTERFACE_HPP_

#include "Roc_ECS.h"
#include "RocLogger/RocLogger.hpp"

class Object
{
protected:
    Entity m_entity;

public:
    Object()
    {
        Coordinator* cd = Coordinator::Get();
        m_entity = cd->CreateEntity();
        LogTrace("Created Entity " + std::to_string(m_entity));
    }

    ~Object()
    {
        Coordinator* cd = Coordinator::Get();
        cd->DestroyEntity(m_entity);
    }
};

#endif
