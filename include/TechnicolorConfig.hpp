#pragma once

#include "TechnicolorStyle.hpp"
#include "TechnicolorLightsGrouping.hpp"
#include "TechnicolorTransition.hpp"

#include "beatsaber-hook/shared/config/config-utils.hpp"
#include <string>

class TechnicolorConfig {
public:
    bool getEnabled();
    void setEnabled(bool enabled);

    static void updateEnabled(bool enabled);


    TechnicolorStyle lightsStyle = TechnicolorStyle::GRADIENT;
    TechnicolorLightsGrouping lightsGrouping = TechnicolorLightsGrouping::ISOLATED;
    float lightsFrequency = 0.1f;

    TechnicolorStyle sabersStyle = TechnicolorStyle::OFF;
    TechnicolorStyle blocksStyle = TechnicolorStyle::OFF;
    TechnicolorStyle wallsStyle = TechnicolorStyle::GRADIENT;
    TechnicolorStyle bombsStyle = TechnicolorStyle::PURE_RANDOM;

    bool desync = false;
    bool disableGradientBackground = false;

    void load(rapidjson::Document& document);
    void save(ConfigDocument& document);

    static std::string saveStyle(TechnicolorStyle style);
    static std::string saveLightsGrouping(TechnicolorLightsGrouping grouping);

    static TechnicolorStyle loadStyle(std::string_view str);
    static std::vector<std::string>& getPossibleStyleValues();
    static std::vector<std::string>& getPossibleGroupingValues();
    static TechnicolorLightsGrouping loadGrouping(std::string_view str);
private:
    bool enabled = true;
};
