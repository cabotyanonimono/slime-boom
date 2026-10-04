#include "pch.h"
#include "object_disabler.h"

#include "game_object.h"

void ObjectDisabler::OnStart()
{
    GameObject()->SetActive(false);
}

CEREAL_REGISTER_TYPE(ObjectDisabler)