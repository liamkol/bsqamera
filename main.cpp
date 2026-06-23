#include "main.hpp"
#include "config.hpp"
#include "camera_manager.hpp"
#include "presets.hpp"

#include "scotland2/shared/modloader.h"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "custom-types/shared/register.hpp"
#include "bsml/shared/BSML.hpp"

namespace Qamera {
    void InstallHooks();
    void RegisterSettingsMenuTab();
}

static modloader::ModInfo modInfo{ID, VERSION, 0};

extern "C" void setup(CModInfo* info) {
    *info = modInfo.to_c();

    Paper::Logger::RegisterFileContextId("Qamera");
    LOG_INFO("Qamera setup() -- BS 1.37.0 build");

    Qamera::getConfig().Load();
    Qamera::PresetStore::get().Load();
}

extern "C" void late_load() {
    LOG_INFO("Qamera late_load()");

    custom_types::Register::AutoRegister();

    Qamera::InstallHooks();
    Qamera::RegisterSettingsMenuTab();

    Qamera::g_initialized = true;
    LOG_INFO("Qamera ready");
}
