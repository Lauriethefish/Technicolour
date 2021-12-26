#pragma once

#include "UnityEngine/Color.hpp"
#include <vector>
#include <optional>

#include "TechnicolorConfig.hpp"
#include "sombrero/shared/ColorUtils.hpp"

#include "GlobalNamespace/LightSwitchEventEffect.hpp"

#include "beatsaber-hook/shared/utils/typedefs-array.hpp"

class GradientController {
private:
    std::optional<Sombrero::FastColor> rainbowSaberColors[2];

    Sombrero::FastColor gradientColor;
    Sombrero::FastColor gradientLeftColor;
    Sombrero::FastColor gradientRightColor;

    std::vector<Sombrero::FastColor> leftSaberPalette;
    std::vector<Sombrero::FastColor> rightSaberPalette;

    Array<GlobalNamespace::LightSwitchEventEffect*>* lightSwitchEventEffects;

    float lastTime = 0;
    float h = 0;

    float currentTime;

    Sombrero::FastColor randomCycleLeft[2];
    Sombrero::FastColor randomCycleRight[2];

    float getTimeMult();
    float getTimeGlobalMult();
    void rainbowLights();
    void rainbowGradientBackground();
    void rainbowNotes();
    void rainbowWalls();
    void rainbowBombs();
    void rainbowSabers();

    void paletteTick();
    void gradientTick();
    void setupWarmCold();

    void randomTick();
    void randomCycleNext();
    void setupRandom();
    
public:
    bool match;
    float mismatchSpeedOffset;

    void update();
    void initialiseGradients();
}; 
