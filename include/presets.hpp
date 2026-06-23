#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace Qamera {

    struct Preset {
        std::string name;
        float posX = 0.0f, posY = 1.6f, posZ = -2.0f;
        float rotX = 0.0f, rotY = 0.0f, rotZ = 0.0f;
        float fov = 60.0f;
        float positionSmoothing = 0.15f;
        float rotationSmoothing = 0.15f;
        bool mirrorHeadset = false;
    };

    // Save/load named camera presets to disk as JSON, separate from the
    // main config file so a single mod-data folder can hold many setups.
    class PresetStore {
    public:
        static PresetStore& get() {
            static PresetStore instance;
            return instance;
        }

        void Load();
        void Save();

        std::vector<std::string> Names() const;
        const Preset* Find(const std::string& name) const;

        void Upsert(const Preset& preset);
        bool Remove(const std::string& name);

    private:
        std::unordered_map<std::string, Preset> presets;
        std::string PathOnDisk() const;
    };
}
