#pragma once

// Include the modloader header, which allows us to tell the modloader which mod
// this is, and the version etc.
#include "scotland2/shared/modloader.h"

// beatsaber-hook is a modding framework that lets us call functions and fetch
// field values from in the game It also allows creating objects, configuration,
// and importantly, hooking methods to modify their values
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/logging.hpp"

#include "HMUI/ViewController.hpp"
#include "HMUI/Touchable.hpp"
#include "bsml/shared/BSML.hpp"


#include "paper/shared/logger.hpp"

#include "_config.hpp"
#include "config.hpp"

void DidActivate(HMUI::ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);

Configuration &getConfig();

/// @brief Stores the ID and version of our mod, and is sent to the modloader upon startup
modloader::ModInfo modInfo{MOD_ID, VERSION, 0};

/// @brief A logger, useful for printing debug messages
/// @return
static constexpr auto Logger = Paper::ConstLoggerContext(MOD_ID "_" VERSION);

#define MOD_EXPORT __attribute__((visibility("default")))
#ifdef __cplusplus
#define MOD_EXPORT_FUNC extern "C" MOD_EXPORT
#else
#define MOD_EXPORT_FUNC MOD_EXPORT
#endif