#include "main.hpp"
#include "config.hpp"
#include "camera_manager.hpp"
#include "presets.hpp"

#include "custom-types/shared/macros.hpp"
#include "bsml/shared/BSML.hpp"
#include "bsml/shared/BSML-Lite.hpp"
#include "bsml/shared/Helpers/getters.hpp"

#include "HMUI/ViewController.hpp"

DEFINE_TYPE(Qamera, SettingsViewController);

// BSML markup for Settings -> Mods -> Qamera. Sliders write straight to
// config on change; "Apply" rebuilds the camera so position/FOV changes
// take effect immediately instead of waiting for the next scene load.
static constexpr auto QAMERA_MENU_BSML = R"(
<vertical>
  <horizontal pad-top='2'>
    <toggle text='Enabled' value='enabled' apply-on-change='true'/>
    <toggle text='Mirror headset view' value='mirrorHeadset' apply-on-change='true'/>
  </horizontal>

  <text text='Position (X / Y / Z)' align='Center'/>
  <horizontal>
    <slider min='-10' max='10' increment='0.05' value='posX' apply-on-change='true'/>
    <slider min='-10' max='10' increment='0.05' value='posY' apply-on-change='true'/>
    <slider min='-10' max='10' increment='0.05' value='posZ' apply-on-change='true'/>
  </horizontal>

  <text text='Rotation (X / Y / Z, degrees)' align='Center'/>
  <horizontal>
    <slider min='-180' max='180' increment='1' value='rotX' apply-on-change='true'/>
    <slider min='-180' max='180' increment='1' value='rotY' apply-on-change='true'/>
    <slider min='-180' max='180' increment='1' value='rotZ' apply-on-change='true'/>
  </horizontal>

  <text text='Field of view' align='Center'/>
  <slider min='30' max='120' increment='1' value='fov' apply-on-change='true'/>

  <text text='Position / rotation smoothing' align='Center'/>
  <horizontal>
    <slider min='0' max='2' increment='0.05' value='posSmoothing' apply-on-change='true'/>
    <slider min='0' max='2' increment='0.05' value='rotSmoothing' apply-on-change='true'/>
  </horizontal>

  <horizontal pad-top='2'>
    <dropdown-list-setting value='activePreset' choices='presetNames' apply-on-change='true'/>
    <button text='Load' on-click='loadPreset'/>
    <button text='Save As...' on-click='saveCurrentAsPreset'/>
  </horizontal>
</vertical>
)";

namespace Qamera {

    void PushConfigToCamera() {
        auto& cfg = getConfig();
        auto& cam = CameraManager::get();
        cam.SetTargetPose(
            {cfg.PosX.GetValue(), cfg.PosY.GetValue(), cfg.PosZ.GetValue()},
            {cfg.RotX.GetValue(), cfg.RotY.GetValue(), cfg.RotZ.GetValue()}
        );
        cam.SetFov(cfg.Fov.GetValue());
        cam.SetSmoothing(cfg.PositionSmoothing.GetValue(), cfg.RotationSmoothing.GetValue());
        cam.SetMirrorHeadset(cfg.MirrorHeadset.GetValue());
        cam.RebuildCamera();
    }
}

DEFINE_TYPE(Qamera, SettingsViewController) {

    custom_types::Helpers::Coroutine Qamera::SettingsViewController::DidActivate(
        bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {

        if (firstActivation) {
            BSML::parse_and_construct(QAMERA_MENU_BSML, this->get_transform(), this);
        }
        co_return;
    }
}

namespace Qamera {
    void RegisterSettingsMenuTab() {
        BSML::Register::RegisterSettingsMenu<SettingsViewController*>("Qamera");
        LOG_INFO("Qamera settings tab registered (Settings -> Mods -> Qamera)");
    }
}
