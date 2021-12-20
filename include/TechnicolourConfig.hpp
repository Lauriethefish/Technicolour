#pragma once

#include "TechnicolourStyle.hpp"
#include "TechnicolourLightsGrouping.hpp"
#include "TechnicolourTransition.hpp"

#include "beatsaber-hook/shared/config/config-utils.hpp"
#include <string>

class TechnicolourConfig {
public:
    bool getEnabled();
    void setEnabled(bool enabled);

    static void updateEnabled(bool enabled);


    TechnicolourStyle lightsStyle = TechnicolourStyle::PURE_RANDOM;
    TechnicolourLightsGrouping lightsGrouping = TechnicolourLightsGrouping::ISOLATED;
    float lightsFrequency = 0.1f;

    TechnicolourStyle sabersStyle = TechnicolourStyle::GRADIENT;
    TechnicolourStyle blocksStyle = TechnicolourStyle::GRADIENT;
    TechnicolourStyle wallsStyle = TechnicolourStyle::GRADIENT;
    TechnicolourStyle bombsStyle = TechnicolourStyle::PURE_RANDOM;

    bool desync = false;
    bool disableGradientBackground = false;

    void load(rapidjson::Document& document);
    void save(ConfigDocument& document);

    static std::string saveStyle(TechnicolourStyle style);
    static std::string saveLightsGrouping(TechnicolourLightsGrouping grouping);

    static TechnicolourStyle loadStyle(std::string_view str);
    static std::vector<std::string>& getPossibleStyleValues();
    static std::vector<std::string>& getPossibleGroupingValues();
    static TechnicolourLightsGrouping loadGrouping(std::string_view str);
private:
    bool enabled = true;
};