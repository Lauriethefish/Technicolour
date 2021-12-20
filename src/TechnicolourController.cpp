#include "TechnicolourController.hpp"
#include <cmath>

Sombrero::FastColor TechnicolourController::getLerpedFromVector(const std::vector<Sombrero::FastColor>& colours, float time) {
    float tm = fmod(time, colours.size());
    int t0 = floor(tm);
    int t1 = ceil(tm);
    if(t1 > colours.size()) {
        t1 = 0;
    }

    return Sombrero::FastColor::Lerp(colours[t0], colours[t1], fmod(tm, 1.0f));
}

Sombrero::FastColor TechnicolourController::getTechnicolour(bool warm, float time, TechnicolourStyle style, TechnicolourTransition transition) {
    switch (style)
    {
        case TechnicolourStyle::PURE_RANDOM:
            return Sombrero::FastColor::HSVToRGB(TechnicolourController::randFloat(), 1.0f, 1.0f);

        case TechnicolourStyle::WARM_COLD:
            return warm ? TechnicolourController::getWarmTechnicolour(time, transition) : TechnicolourController::getColdTechnicolour(time, transition);

        default:
            return Sombrero::FastColor::get_white();
    }
}

Sombrero::FastColor getPalettedTechnicolour(float time, TechnicolourTransition transition, std::vector<Sombrero::FastColor> palette) {
    switch (transition)
    {
        case TechnicolourTransition::FLAT:
            return TechnicolourController::getRandomFromVector(palette);

        case TechnicolourTransition::SMOOTH:
            return TechnicolourController::getLerpedFromVector(palette, time);

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

std::vector<Sombrero::FastColor> TechnicolourController::getWarmPalette() {
    return warmPalette;
}

std::vector<Sombrero::FastColor> TechnicolourController::getColdPalette() {
    return coldPalette;
}

Sombrero::FastColor TechnicolourController::getWarmTechnicolour(float time, TechnicolourTransition transition) {
    return getPalettedTechnicolour(time, transition, warmPalette);
}

Sombrero::FastColor TechnicolourController::getColdTechnicolour(float time, TechnicolourTransition transition) {
    return getPalettedTechnicolour(time, transition, coldPalette);
}

Sombrero::FastColor TechnicolourController::getRandomFromVector(const std::vector<Sombrero::FastColor>& colours) {
    return TechnicolourController::getLerpedFromVector(colours, TechnicolourController::randFloat());
}