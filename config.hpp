#pragma once

#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(QameraConfig,
    CONFIG_VALUE(Enabled, bool, "Enabled", true);

    // Free-cam position offset (meters, relative to playspace origin)
    CONFIG_VALUE(PosX, float, "PosX", 0.0f);
    CONFIG_VALUE(PosY, float, "PosY", 1.6f);
    CONFIG_VALUE(PosZ, float, "PosZ", -2.0f);

    // Free-cam rotation offset (degrees, euler)
    CONFIG_VALUE(RotX, float, "RotX", 0.0f);
    CONFIG_VALUE(RotY, float, "RotY", 0.0f);
    CONFIG_VALUE(RotZ, float, "RotZ", 0.0f);

    CONFIG_VALUE(Fov, float, "Fov", 60.0f);

    CONFIG_VALUE(PositionSmoothing, float, "PositionSmoothing", 0.15f);
    CONFIG_VALUE(RotationSmoothing, float, "RotationSmoothing", 0.15f);

    CONFIG_VALUE(MirrorHeadset, bool, "MirrorHeadset", false);

    CONFIG_VALUE(ActivePreset, std::string, "ActivePreset", "Default");
)

namespace Qamera {
    inline QameraConfig& getConfig() {
        static QameraConfig config(Configuration::globalConfig);
        return config;
    }
}
