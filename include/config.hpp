#pragma once

#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(ModConfig,
    CONFIG_VALUE(Active, bool, "Activate Mod", true);
    CONFIG_VALUE(OneHand, bool, "One Hand", false);
    CONFIG_VALUE(LeftHanded, bool, "Left Handed", false);
)