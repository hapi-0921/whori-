#pragma once
#include <string>
#include <vector>
#include <json.hpp>
using json = nlohmann::json;

class JsonManager
{
public:
	struct TargetData
	{
		std::string modelPath;
		std::string spritePath;
		std::string name;
		std::string startN;
		std::string endN;
	};


    JsonManager() = default;
    ~JsonManager() = default;

    bool LoadTargets(const std::string& path);

    const std::vector<TargetData>& GetTargets() const { return targets; }
    const std::string& GetFirstName() const { return firstName; }


private:
    std::vector<TargetData> targets;
    std::string firstName = "‚ ";
};