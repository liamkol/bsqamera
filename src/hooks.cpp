#include "main.hpp"
#include "camera_manager.hpp"
#include "config.hpp"

#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"

#include "MainSystemInit.hpp"
#include "VRRenderingParamsSetup.hpp"
#include "UnityEngine/Transform.hpp"

// NOTE on retargeting from 1.40.8 -> 1.37.0:
// MainSystemInit::Init and VRRenderingParamsSetup::Start/Update have been
// stable hook points across the 1.3x-1.4x line, but their exact parameter
// lists are generated per-game-version by bs-cordl. If `qpm restore` pulls
// 1.37.0 headers and these signatures don't match exactly, the fix is just
// updating the MAKE_HOOK_MATCH line below to match what's in
// extern/includes/System/MainSystemInit.hpp for this game build -- the
// hook *target* (this function, at this point in startup) stays correct.

MAKE_HOOK_MATCH(MainSystemInit_Init, &MainSystemInit::Init, void, MainSystemInit* self) {
    MainSystemInit_Init(self);
    LOG_DEBUG("MainSystemInit::Init -- game systems ready");
}

MAKE_HOOK_MATCH(VRRenderingParamsSetup_Start, &VRRenderingParamsSetup::Start, void, VRRenderingParamsSetup* self) {
    VRRenderingParamsSetup_Start(self);

    auto* headTransform = self->get_transform();
    Qamera::CameraManager::get().Init(headTransform);
    LOG_DEBUG("VRRenderingParamsSetup::Start -- camera rig anchored");
}

MAKE_HOOK_MATCH(VRRenderingParamsSetup_Update, &VRRenderingParamsSetup::Update, void, VRRenderingParamsSetup* self) {
    VRRenderingParamsSetup_Update(self);

    if (!Qamera::g_initialized || !Qamera::getConfig().Enabled.GetValue()) {
        return;
    }

    Qamera::CameraManager::get().Tick(UnityEngine::Time::get_deltaTime());
}

namespace Qamera {
    void InstallHooks() {
        INSTALL_HOOK(getLogger(), MainSystemInit_Init);
        INSTALL_HOOK(getLogger(), VRRenderingParamsSetup_Start);
        INSTALL_HOOK(getLogger(), VRRenderingParamsSetup_Update);
        LOG_INFO("Qamera hooks installed");
    }
}
