#include "TechnicolorConfig.hpp"
#include "chroma/shared/CoreAPI.hpp"

std::vector<std::string>& TechnicolorConfig::getPossibleStyleValues() {
    static std::vector<std::string> values = {
        "Off",
        "Gradient",
        "Pure Random",
        "Warm Cold"
    };
    return values;
}

std::string TechnicolorConfig::saveStyle(TechnicolorStyle style) {
    switch(style) {
        case TechnicolorStyle::GRADIENT:
            return "Gradient";
        case TechnicolorStyle::OFF:
            return "Off";
        case TechnicolorStyle::PURE_RANDOM:
            return "Pure Random";
        case TechnicolorStyle::WARM_COLD:
            return "Warm Cold";
    }
}

TechnicolorStyle TechnicolorConfig::loadStyle(std::string_view str) {
    if(str == "Gradient") {
        return TechnicolorStyle::GRADIENT;
    }   else if(str == "Off") {
        return TechnicolorStyle::OFF;
    }   else if(str == "Pure Random") {
        return TechnicolorStyle::PURE_RANDOM;
    }   else if(str == "Warm Cold") {
        return TechnicolorStyle::WARM_COLD;
    }
    // Fall back to OFF
    return TechnicolorStyle::OFF;
}

std::vector<std::string>& TechnicolorConfig::getPossibleGroupingValues() {
    static std::vector<std::string> values = {
        "Isolated",
        "Isolated Group",
        "Standard"
    };
    return values;
}

std::string TechnicolorConfig::saveLightsGrouping(TechnicolorLightsGrouping grouping) {
    switch(grouping) {
        case TechnicolorLightsGrouping::ISOLATED:
            return "Isolated";
        case TechnicolorLightsGrouping::ISOLATED_GROUP:
            return "Isolated Group";
        case TechnicolorLightsGrouping::STANDARD:
            return "Standard";
    }
}

TechnicolorLightsGrouping TechnicolorConfig::loadGrouping(std::string_view str) {
    if(str == "Isolated") {
        return TechnicolorLightsGrouping::ISOLATED;
    }   else if(str == "Isolated Group") {
        return TechnicolorLightsGrouping::ISOLATED_GROUP;
    }   else if(str == "Standard") {
        return TechnicolorLightsGrouping::STANDARD;
    }

    // Fall back to STANDARD
    return TechnicolorLightsGrouping::STANDARD;
}

void TechnicolorConfig::load(ConfigDocument& document) {
    enabled = document["enabled"].GetBool();
    lightsStyle = TechnicolorConfig::loadStyle(document["lightsStyle"].GetString());
    lightsGrouping = TechnicolorConfig::loadGrouping(document["lightsGrouping"].GetString());

    sabersStyle = TechnicolorConfig::loadStyle(document["sabersStyle"].GetString());
    blocksStyle = TechnicolorConfig::loadStyle(document["blocksStyle"].GetString());
    wallsStyle = TechnicolorConfig::loadStyle(document["wallsStyle"].GetString());
    bombsStyle = TechnicolorConfig::loadStyle(document["bombsStyle"].GetString());

    desync = document["desync"].GetBool();
    disableGradientBackground = document["disableGradientBackground"].GetBool();
    lightsFrequency = document["lightsFrequency"].GetFloat();
}

void TechnicolorConfig::save(ConfigDocument& document) {
    auto& alloc = document.GetAllocator();

    document.AddMember("enabled", enabled, alloc);
    document.AddMember("lightsStyle", TechnicolorConfig::saveStyle(lightsStyle), alloc);
    document.AddMember("lightsGrouping", TechnicolorConfig::saveLightsGrouping(lightsGrouping), alloc);

    document.AddMember("sabersStyle", TechnicolorConfig::saveStyle(sabersStyle), alloc);
    document.AddMember("blocksStyle", TechnicolorConfig::saveStyle(blocksStyle), alloc);
    document.AddMember("wallsStyle", TechnicolorConfig::saveStyle(wallsStyle), alloc);
    document.AddMember("bombsStyle", TechnicolorConfig::saveStyle(bombsStyle), alloc);

    document.AddMember("desync", desync, alloc);
    document.AddMember("disableGradientBackground", disableGradientBackground, alloc);
    document.AddMember("lightsFrequency", lightsFrequency, alloc);
}

bool TechnicolorConfig::getEnabled() {
    return enabled;
}

void TechnicolorConfig::setEnabled(bool enabled) {
    bool changed = this->enabled != enabled;
    if(changed) {
        TechnicolorConfig::updateEnabled(enabled);
    }

    this->enabled = enabled;
}

void TechnicolorConfig::updateEnabled(bool enabled) {
    ModInfo modInfo;
    modInfo.id = "technicolor-with-a-u-screw-you";

    if(enabled) {
        Chroma::CoreAPI::addForceEnableChromaHooks(modInfo);
    }   else    {
        Chroma::CoreAPI::removeForceEnableChromaHooks(modInfo);
    }
}
