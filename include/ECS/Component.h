#ifndef _ROC_COMPONENT_H_
#define _ROC_COMPONENT_H_

#include <cstdint>
#include <bitset>

using ComponentType = std::uint16_t;

const ComponentType MAX_COMPONENTS = 64;

using Signature = std::bitset<MAX_COMPONENTS>;

/**
 * A quick base class, just so that any components
 * created all implement certain Constructors and have
 * a public "isNull" boolean.
*/
class Component
{
private:
    bool mIsNull;

public:
    Component(bool null = false) {mIsNull = null;}
    bool isNull() {return mIsNull;}
};

#endif