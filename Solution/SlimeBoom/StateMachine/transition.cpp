#include "pch.h"
#include "transition.h"

bool Transition::Evaluate() const
{
    for (const auto &condition : conditions)
        if (!condition->Evaluate())
            return false;

    return true;
}
