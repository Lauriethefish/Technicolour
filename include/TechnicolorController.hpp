#pragma once
#include "UnityEngine/Color.hpp"
#include "TechnicolorStyle.hpp"
#include "TechnicolorTransition.hpp"
#include <vector>
#include "sombrero/shared/ColorUtils.hpp"

class TechnicolorController {
public:
    static Sombrero::FastColor getTechnicolor(bool warm, float time, TechnicolorStyle style, TechnicolorTransition transition = TechnicolorTransition::FLAT);
    static Sombrero::FastColor getLerpedFromVector(const std::vector<Sombrero::FastColor>& colours, float time);

    static std::vector<Sombrero::FastColor> getWarmPalette();
    static std::vector<Sombrero::FastColor> getColdPalette();

    static Sombrero::FastColor getWarmTechnicolor(float time, TechnicolorTransition transition);
    static Sombrero::FastColor getColdTechnicolor(float time, TechnicolorTransition transition);
    static Sombrero::FastColor getRandomFromVector(const std::vector<Sombrero::FastColor>& colours);

    static inline float randFloat() {
        return ((float) rand() / (float) RAND_MAX);
    }
};
