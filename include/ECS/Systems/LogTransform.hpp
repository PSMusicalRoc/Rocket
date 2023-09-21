#ifndef _ROC_LOGTRANSFORM_SYSTEM_HPP_
#define _ROC_LOGTRANSFORM_SYSTEM_HPP_

#include "Roc_ECS.h"
#include "RocLogger/RocLogger.hpp"
#include "Roc_GL/CoordinateSystem.hpp"

class LogTransform : public System
{
public:
    void Do()
    {
        Coordinator* cd = Coordinator::Get();
        for (Entity e : mEntities)
        {
            Transform& t = cd->GetComponent<Transform>(e);
            std::string logstr;
            logstr += "Entity ";
            logstr += std::to_string(e);
            logstr += ", [x=";
            logstr += std::to_string(t.x);
            logstr += ",y=";
            logstr += std::to_string(t.y);
            logstr += "]";
            LogTrace(logstr);

            std::string glstr;
            glstr += "GLCoord: [x=";
            glstr += std::to_string(EX2GL(t.x));
            glstr += ", y=";
            glstr += std::to_string(EY2GL(t.y));
            glstr += "]";
            LogTrace(glstr);
        }
    }

    Signature GetSignature() override
    {
        Signature sig;
        Coordinator* cd = Coordinator::Get();
        sig[cd->GetComponentType<Transform>()].flip();
        return sig;
    }
};

#endif
