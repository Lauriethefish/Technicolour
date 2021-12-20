#pragma once
#include "UnityEngine/Color.hpp"
#include "TechnicolourStyle.hpp"
#include "TechnicolourTransition.hpp"
#include <vector>
#include "sombrero/shared/ColorUtils.hpp"

class TechnicolourController {
public:
    static Sombrero::FastColor getTechnicolour(bool warm, float time, TechnicolourStyle style, TechnicolourTransition transition = TechnicolourTransition::FLAT);
    static Sombrero::FastColor getLerpedFromVector(std::vector<Sombrero::FastColor> colours, float time);

    static std::vector<Sombrero::FastColor> getWarmPalette();
    static std::vector<Sombrero::FastColor> getColdPalette();

    static Sombrero::FastColor getWarmTechnicolour(float time, TechnicolourTransition transition);
    static Sombrero::FastColor getColdTechnicolour(float time, TechnicolourTransition transition);
    static Sombrero::FastColor getRandomFromVector(std::vector<Sombrero::FastColor> colours);

    static float randFloat();
};