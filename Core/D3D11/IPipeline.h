#pragma once
#include "Framework.h"

class IPipeline
{
public:
    IPipeline()
    {
        id = GUID_Generator::Generate();
    }
    virtual ~IPipeline() = default;

    auto GetID() const -> const uint& { return id; }

private:
    uint id;
};