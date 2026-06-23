#pragma once

#include "beatsaber-hook/shared/utils/logging.hpp"

static const Logger& getLogger() {
    static auto logger = new Paper::ConstLoggerContext<"Qamera">();
    return *reinterpret_cast<Logger*>(logger);
}

// Simple macro wrappers, swap for Paper::Logger calls if your
// beatsaber-hook version's logging API differs after qpm restore.
#define LOG_INFO(...)  getLogger().info(__VA_ARGS__)
#define LOG_DEBUG(...) getLogger().debug(__VA_ARGS__)
#define LOG_ERROR(...) getLogger().error(__VA_ARGS__)

namespace Qamera {
    // Set once setup() runs; flips true after late_load() finishes
    // registering hooks + BSML settings menu.
    inline bool g_initialized = false;
}
