#include "JsonManager.h"
#include <fstream>
#include <iostream>

bool JsonManager::LoadTargets(const std::string& path)
{
    std::ifstream file(path);

    json j;
    file >> j;

    if (j.contains("firstStartN")) {
        firstName = j["firstStartN"].get<std::string>();
    }

    targets.clear();

    if (j.contains("targets") && j["targets"].is_array())
    {
        for (auto& t : j["targets"])
        {
            TargetData data;
            data.modelPath = t.value("model", t.value("file", ""));
            data.spritePath = t.value("sprite", "");
            data.name = t.value("name", "");
            data.startN = t.value("startN", "");
            data.endN = t.value("endN", "");

            if (!data.modelPath.empty() && !data.name.empty()) {
                targets.push_back(data);
            }
        }
    }


    return true;
}
