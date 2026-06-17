#pragma once
#include "condition_base.h"

class Transition
{
public:
    std::vector<std::shared_ptr<ConditionBase>> conditions;

    bool Evaluate() const;
};
