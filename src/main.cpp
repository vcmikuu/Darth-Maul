#include "main.hpp"
#include "GlobalNamespace/BeatmapObjectSpawnMovementData.hpp"
#include "GlobalNamespace/MainMenuViewController.hpp"
#include "HMUI/CurvedTextMeshPro.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Quaternion.hpp"
#include "UnityEngine/Vector3.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "HMUI/Touchable.hpp"
#include "config-utils/shared/config-utils.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "config.hpp"
#include "System/Action.hpp"
#include "GlobalNamespace/PlayerTransforms.hpp"
#include "GlobalNamespace/Saber.hpp"
#include "GlobalNamespace/SaberClashEffect.hpp"
#include "scotland2/shared/modloader.h"

using namespace UnityEngine;
using namespace GlobalNamespace;


float xa = 0.0f;
float ya = 0.0f;
float za = 0.0f;

float xb = 0.0f;
float yb = 0.0f;
float zb = 0.0f;

float xr = 0.0f;
float yr = 0.0f;
float zr = 0.0f;

MAKE_HOOK_MATCH(SaberClashEffect_LateUpdate, &SaberClashEffect::LateUpdate, void,
    SaberClashEffect* self
) {
    if(getModConfig().Active.GetValue()){
        return;
    } else{
        SaberClashEffect_LateUpdate(self);
    }
};

MAKE_HOOK_MATCH(PlayerTransforms_Update, &PlayerTransforms::Update, void, PlayerTransforms* self) {
    if(getModConfig().Active.GetValue()){
        if(getModConfig().OneHand.GetValue()){
            if(getModConfig().LeftHanded.GetValue()){
                self->_rightHandTransform->get_transform()->set_localPosition({
                    self->_leftHandTransform->get_position().x,
                    self->_leftHandTransform->get_position().y,
                    self->_leftHandTransform->get_position().z
                });

                self->_rightHandTransform->get_transform()->set_eulerAngles({
                    self->_leftHandTransform->get_eulerAngles().x + 180,
                    self->_leftHandTransform->get_eulerAngles().y,
                    self->_leftHandTransform->get_eulerAngles().z,
                });
            } else{
                self->_leftHandTransform->get_transform()->set_localPosition({
                    self->_rightHandTransform->get_position().x,
                    self->_rightHandTransform->get_position().y,
                    self->_rightHandTransform->get_position().z
                });

                self->_leftHandTransform->get_transform()->set_eulerAngles({
                    self->_rightHandTransform->get_eulerAngles().x + 180,
                    self->_rightHandTransform->get_eulerAngles().y,
                    self->_rightHandTransform->get_eulerAngles().z,
                });
            }
            
        } else{
            xa = self->_rightHandTransform->get_position().x;
            ya = self->_rightHandTransform->get_position().y;
            za = self->_rightHandTransform->get_position().z;

            xb = self->_leftHandTransform->get_position().x;
            yb = self->_leftHandTransform->get_position().y;
            zb = self->_leftHandTransform->get_position().z;

            float xAverage = (xa + xb) / 2;
            float yAverage = (ya + yb) / 2;
            float zAverage = (za + zb) / 2;

            Quaternion rotation = UnityEngine::Quaternion::LookRotation(self->_rightHandTransform->get_position(), self->_leftHandTransform->get_position());
        
            self->_rightHandTransform->get_transform()->set_localPosition({xAverage, yAverage, zAverage});
            self->_rightHandTransform->get_transform()->set_eulerAngles(rotation.get_eulerAngles());
            
            self->_leftHandTransform->get_transform()->set_localPosition({xAverage, yAverage, zAverage});
            self->_leftHandTransform->get_transform()->set_eulerAngles({
                rotation.get_eulerAngles().x + 180,
                rotation.get_eulerAngles().y,
                rotation.get_eulerAngles().z * -1,
            });
        }
    }
    
    PlayerTransforms_Update(self);
};


void DidActivate(HMUI::ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling){
    if(firstActivation){
        // Make Touchable
        self->get_gameObject()->AddComponent<HMUI::Touchable*>();

        // Create Container
        auto* container = BSML::Lite::CreateScrollableSettingsContainer(self->get_transform());
        
        // Add Options
        AddConfigValueToggle(container->get_transform(), getModConfig().Active);
        AddConfigValueToggle(container->get_transform(), getModConfig().OneHand);
        AddConfigValueToggle(container->get_transform(), getModConfig().LeftHanded);
    }
}



#pragma region Mod setup
/// @brief Called at the early stages of game loading
/// @param info
/// @return
MOD_EXPORT_FUNC void setup(CModInfo& info) {
    info.id = MOD_ID;
    info.version = VERSION;

    Logger.info("Completed setup!");
}

/// @brief Called later on in the game loading - a good time to install function hooks
/// @return
MOD_EXPORT_FUNC void late_load() {
    il2cpp_functions::Init();
    getModConfig().Init(modInfo);
    BSML::Init();

    BSML::Register::RegisterMainMenu("Darth Maul", "Darth Maul", "Configure some darth maul settings", DidActivate);

    Logger.info("Installing hooks...");

    INSTALL_HOOK(Logger, PlayerTransforms_Update);
    INSTALL_HOOK(Logger, SaberClashEffect_LateUpdate);

    Logger.info("Installed all hooks!");
}
#pragma endregion