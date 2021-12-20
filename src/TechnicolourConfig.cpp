#include "TechnicolourConfig.hpp"
#include "chroma/shared/CoreAPI.hpp"

std::vector<std::string>& TechnicolourConfig::getPossibleStyleValues() {
    static std::vector<std::string> values = {
        "Off",
        "Gradient",
        "Pure Random",
        "Warm Cold"
    };
    return values;
}

std::string TechnicolourConfig::saveStyle(TechnicolourStyle style) {
    switch(style) {
        case TechnicolourStyle::GRADIENT:
            return "Gradient";
        case TechnicolourStyle::OFF:
            return "Off";
        case TechnicolourStyle::PURE_RANDOM:
            return "Pure Random";
        case TechnicolourStyle::WARM_COLD:
            return "Warm Cold";
    }
}

TechnicolourStyle TechnicolourConfig::loadStyle(std::string_view str) {
    if(str == "Gradient") {
        return TechnicolourStyle::GRADIENT;
    }   else if(str == "Off") {
        return TechnicolourStyle::OFF;
    }   else if(str == "Pure Random") {
        return TechnicolourStyle::PURE_RANDOM;
    }   else if(str == "Warm Cold") {
        return TechnicolourStyle::WARM_COLD;
    }
    // Fall back to OFF
    return TechnicolourStyle::OFF;
}

std::vector<std::string>& TechnicolourConfig::getPossibleGroupingValues() {
    static std::vector<std::string> values = {
        "Isolated",
        "Isolated Group",
        "Standard"
    };
    return values;
}

std::string TechnicolourConfig::saveLightsGrouping(TechnicolourLightsGrouping grouping) {
    switch(grouping) {
        case TechnicolourLightsGrouping::ISOLATED:
            return "Isolated";
        case TechnicolourLightsGrouping::ISOLATED_GROUP:
            return "Isolated Group";
        case TechnicolourLightsGrouping::STANDARD:
            return "Standard";
    }
}

TechnicolourLightsGrouping TechnicolourConfig::loadGrouping(std::string_view str) {
    if(str == "Isolated") {
        return TechnicolourLightsGrouping::ISOLATED;
    }   else if(str == "Isolated Group") {
        return TechnicolourLightsGrouping::ISOLATED_GROUP;
    }   else if(str == "Standard") {
        return TechnicolourLightsGrouping::STANDARD;
    }

    // Fall back to STANDARD
    return TechnicolourLightsGrouping::STANDARD;
}

void TechnicolourConfig::load(ConfigDocument& document) {
    enabled = document["enabled"].GetBool();
    lightsStyle = TechnicolourConfig::loadStyle(document["lightsStyle"].GetString());
    lightsGrouping = TechnicolourConfig::loadGrouping(document["lightsGrouping"].GetString());

    sabersStyle = TechnicolourConfig::loadStyle(document["sabersStyle"].GetString());
    blocksStyle = TechnicolourConfig::loadStyle(document["blocksStyle"].GetString());
    wallsStyle = TechnicolourConfig::loadStyle(document["wallsStyle"].GetString());
    bombsStyle = TechnicolourConfig::loadStyle(document["bombsStyle"].GetString());

    desync = document["desync"].GetBool();
    disableGradientBackground = document["disableGradientBackground"].GetBool();
}

void TechnicolourConfig::save(ConfigDocument& document) {
    auto& alloc = document.GetAllocator();

    document.AddMember("enabled", enabled, alloc);
    document.AddMember("lightsStyle", TechnicolourConfig::saveStyle(lightsStyle), alloc);
    document.AddMember("lightsGrouping", TechnicolourConfig::saveLightsGrouping(lightsGrouping), alloc);

    document.AddMember("sabersStyle", TechnicolourConfig::saveStyle(sabersStyle), alloc);
    document.AddMember("blocksStyle", TechnicolourConfig::saveStyle(blocksStyle), alloc);
    document.AddMember("wallsStyle", TechnicolourConfig::saveStyle(wallsStyle), alloc);
    document.AddMember("bombsStyle", TechnicolourConfig::saveStyle(bombsStyle), alloc);

    document.AddMember("desync", desync, alloc);
    document.AddMember("disableGradientBackground", disableGradientBackground, alloc);
}

bool TechnicolourConfig::getEnabled() {
    return enabled;
}

void TechnicolourConfig::setEnabled(bool enabled) {
    bool changed = this->enabled != enabled;
    if(changed) {
        TechnicolourConfig::updateEnabled(enabled);
    }

    this->enabled = enabled;
}

void TechnicolourConfig::updateEnabled(bool enabled) {
    ModInfo modInfo;
    modInfo.id = "technicolour-with-a-u-screw-you";

    if(enabled) {
        Chroma::CoreAPI::addForceEnableChromaHooks(modInfo);
    }   else    {
        Chroma::CoreAPI::removeForceEnableChromaHooks(modInfo);
    }
}