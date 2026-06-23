#include "presets.hpp"
#include "main.hpp"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include <fstream>
#include <sstream>

namespace Qamera {

    std::string PresetStore::PathOnDisk() const {
        return "/sdcard/ModData/com.beatgames.beatsaber/Mods/Qamera/presets.json";
    }

    void PresetStore::Load() {
        presets.clear();

        std::ifstream file(PathOnDisk());
        if (!file.is_open()) {
            LOG_INFO("No presets.json found yet, starting empty");
            return;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();

        rapidjson::Document doc;
        if (doc.Parse(buffer.str().c_str()).HasParseError() || !doc.IsArray()) {
            LOG_ERROR("Failed to parse presets.json, ignoring file");
            return;
        }

        for (auto& entry : doc.GetArray()) {
            if (!entry.IsObject()) continue;
            Preset p;
            p.name = entry["name"].GetString();
            p.posX = entry["posX"].GetFloat();
            p.posY = entry["posY"].GetFloat();
            p.posZ = entry["posZ"].GetFloat();
            p.rotX = entry["rotX"].GetFloat();
            p.rotY = entry["rotY"].GetFloat();
            p.rotZ = entry["rotZ"].GetFloat();
            p.fov = entry["fov"].GetFloat();
            p.positionSmoothing = entry["positionSmoothing"].GetFloat();
            p.rotationSmoothing = entry["rotationSmoothing"].GetFloat();
            p.mirrorHeadset = entry["mirrorHeadset"].GetBool();
            presets[p.name] = p;
        }

        LOG_INFO("Loaded " + std::to_string(presets.size()) + " preset(s)");
    }

    void PresetStore::Save() {
        rapidjson::Document doc;
        doc.SetArray();
        auto& alloc = doc.GetAllocator();

        for (auto& [name, p] : presets) {
            rapidjson::Value entry(rapidjson::kObjectType);
            entry.AddMember("name", rapidjson::Value(p.name.c_str(), alloc), alloc);
            entry.AddMember("posX", p.posX, alloc);
            entry.AddMember("posY", p.posY, alloc);
            entry.AddMember("posZ", p.posZ, alloc);
            entry.AddMember("rotX", p.rotX, alloc);
            entry.AddMember("rotY", p.rotY, alloc);
            entry.AddMember("rotZ", p.rotZ, alloc);
            entry.AddMember("fov", p.fov, alloc);
            entry.AddMember("positionSmoothing", p.positionSmoothing, alloc);
            entry.AddMember("rotationSmoothing", p.rotationSmoothing, alloc);
            entry.AddMember("mirrorHeadset", p.mirrorHeadset, alloc);
            doc.PushBack(entry, alloc);
        }

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer);

        std::ofstream file(PathOnDisk());
        file << buffer.GetString();

        LOG_INFO("Saved " + std::to_string(presets.size()) + " preset(s)");
    }

    std::vector<std::string> PresetStore::Names() const {
        std::vector<std::string> names;
        names.reserve(presets.size());
        for (auto& [name, _] : presets) names.push_back(name);
        return names;
    }

    const Preset* PresetStore::Find(const std::string& name) const {
        auto it = presets.find(name);
        return it == presets.end() ? nullptr : &it->second;
    }

    void PresetStore::Upsert(const Preset& preset) {
        presets[preset.name] = preset;
        Save();
    }

    bool PresetStore::Remove(const std::string& name) {
        bool removed = presets.erase(name) > 0;
        if (removed) Save();
        return removed;
    }
}
