#include "TechnicolorController.hpp"
#include <cmath>

Sombrero::FastColor TechnicolorController::getLerpedFromVector(const std::vector<Sombrero::FastColor>& colours, float time) {
    float tm = fmod(time, colours.size());
    int t0 = floor(tm);
    int t1 = ceil(tm);
    if(t1 >= colours.size()) {
        t1 = 0;
    }

    return Sombrero::FastColor::Lerp(colours[t0], colours[t1], fmod(tm, 1.0f));
}

Sombrero::FastColor TechnicolorController::getTechnicolor(bool warm, float time, TechnicolourStyle style, TechnicolourTransition transition) {
    switch (style)
    {
        case TechnicolorStyle::PURE_RANDOM:
            return Sombrero::FastColor::HSVToRGB(TechnicolorController::randFloat(), 1.0f, 1.0f);

        case TechnicolorStyle::WARM_COLD:
            return warm ? TechnicolorController::getWarmTechnicolor(time, transition) : TechnicolorController::getColdTechnicolor(time, transition);

        default:
            return Sombrero::FastColor::get_white();
    }
}

Sombrero::FastColor getPalettedTechnicolor(float time, TechnicolorTransition transition, std::vector<Sombrero::FastColor>& palette) {
    switch (transition)
    {
        case TechnicolorTransition::FLAT:
            return TechnicolorController::getRandomFromVector(palette);

        case TechnicolorTransition::SMOOTH:
            return TechnicolorController::getLerpedFromVector(palette, time);

        default:
            return Sombrero::FastColor::get_white();
    }
}

static std::vector<Sombrero::FastColor> warmPalette = {
    Sombrero::FastColor { 1, 0, 0 },
    Sombrero::FastColor{ 1, 0, 1 },
    Sombrero::FastColor{ 1, 0.6f, 0 },
    Sombrero::FastColor{ 1, 0, 0.4f }
};

static std::vector<Sombrero::FastColor> coldPalette = {
    Sombrero::FastColor{ 0, 0.501f, 1 },
    Sombrero::FastColor{ 0, 1, 0 },
    Sombrero::FastColor{ 0, 0, 1 },
    Sombrero::FastColor{ 0, 1, 0.8f }
};

std::vector<Sombrero::FastColor> TechnicolorController::getWarmPalette() {
    return warmPalette;
}

std::vector<Sombrero::FastColor> TechnicolorController::getColdPalette() {
    return coldPalette;
}

Sombrero::FastColor TechnicolorController::getWarmTechnicolor(float time, TechnicolorTransition transition) {
    return getPalettedTechnicolor(time, transition, warmPalette);
}

Sombrero::FastColor TechnicolorController::getColdTechnicolor(float time, TechnicolorTransition transition) {
    return getPalettedTechnicolor(time, transition, coldPalette);
}

Sombrero::FastColor TechnicolorController::getRandomFromVector(const std::vector<Sombrero::FastColor>& colours) {
    return TechnicolorController::getLerpedFromVector(colours, TechnicolorController::randFloat());
}
