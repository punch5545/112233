#pragma once
#include "framework.h"

class ISubsystem
{
public:
    ISubsystem(class Context* context)
        : context(context) 
    {}
    virtual ~ISubsystem() = default;

    virtual const bool Initialize() = 0;

protected:
    Context* context;
};