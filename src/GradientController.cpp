#include "GradientController.hpp"
#include "main.hpp"
#include "Hooks.hpp"
#include "UnityEngine/Time.hpp"
#include "TechnicolourController.hpp"
#include "TechnicolourStyle.hpp"

#include "chroma/shared/LightAPI.hpp"
#include "chroma/shared/NoteAPI.hpp"
#include "chroma/shared/ObstacleAPI.hpp"
#include "chroma/shared/SaberAPI.hpp"
#include "chroma/shared/BombAPI.hpp"

#include "GlobalNamespace/SaberType.hpp"
#include "GlobalNamespace/ILightWithId.hpp"

#include "UnityEngine/Resources.hpp"

#include <cmath>

float GradientController::getTimeMult() {
    return getConfig().lightsFrequency;
}

float GradientController::getTimeGlobalMult() {
    return (getConfig().lightsFrequency / 2.0f) + 0.7f;
}

void GradientController::initialiseGradients() {
    switch (getConfig().sabersStyle)
    {
        case TechnicolourStyle::PURE_RANDOM:
            setupRandom();
            break;

        case TechnicolourStyle::WARM_COLD:
            setupWarmCold();
            break;
        default:
            break;
    }
}

void GradientController::update() {
    float time = UnityEngine::Time::get_time();
    currentTime = time;
    gradientColor = Sombrero::FastColor::HSVToRGB(fmod(time * getTimeGlobalMult(), 1.0f), 1.0f, 1.0f);
    gradientLeftColor = Sombrero::FastColor::HSVToRGB(fmod(time * getTimeMult() + mismatchSpeedOffset, 1.0f), 1.0f, 1.0f);
    gradientRightColor = Sombrero::FastColor::HSVToRGB(fmod(time * getTimeMult(), 1.0f), 1.0f, 1.0f);

    TechnicolourConfig& config = getConfig();
    if (config.lightsStyle == TechnicolourStyle::GRADIENT)
    {
        rainbowLights();
    }

    if (config.lightsStyle != TechnicolourStyle::OFF && !config.disableGradientBackground)
    {
        rainbowGradientBackground();
    }
    if (config.blocksStyle == TechnicolourStyle::GRADIENT)
    {
        rainbowNotes();
    }

    if (config.wallsStyle == TechnicolourStyle::GRADIENT)
    {
        rainbowWalls();
    }

    if (config.bombsStyle == TechnicolourStyle::GRADIENT)
    {
        rainbowBombs();
    }

    if (config.sabersStyle != TechnicolourStyle::OFF)
    {
        switch (config.sabersStyle)
        {
            case TechnicolourStyle::GRADIENT:
                gradientTick();
                break;

            case TechnicolourStyle::PURE_RANDOM:
                randomTick();
                break;

            case TechnicolourStyle::WARM_COLD:
                paletteTick();
                break;
            default:
                break;
        }

        rainbowSabers();
    }
}
static int lightingEventTypes[] = {
    0,
    1,
    2,
    3,
    4,
    6,
    7
};

void GradientController::rainbowLights() {
    if (getConfig().lightsGrouping == TechnicolourLightsGrouping::ISOLATED)
    {
        if(!lightSwitchEventEffects) {
            lightSwitchEventEffects = UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::LightSwitchEventEffect*>();
        }
        Chroma::LightAPI::setAllLightingColorsSafe(false, Chroma::LightAPI::LSEData{gradientLeftColor, gradientRightColor, gradientLeftColor, gradientRightColor});

        for (int i = 0; i < lightSwitchEventEffects->Length(); i++)
        {
            GlobalNamespace::LightSwitchEventEffect* lightSwitchEventEffect = lightSwitchEventEffects->values[i];
            auto lights =  Chroma::LightAPI::getLightsSafe(lightSwitchEventEffect);
            if(!lights) {
                getLogger().warning("Lights were nullopt for light switch event!");
            }

            int latestValue = getLatestLightEventDataValues()[lightSwitchEventEffect->lightsID];
            bool leftColor = isColor0EventDataValue(latestValue);
            for(GlobalNamespace::ILightWithId* light : lights.value()) {
                float seed = abs(reinterpret_cast<System::Object*>(light)->GetHashCode()) % 1000;
                seed *= 0.001f;
                Sombrero::FastColor colorLeft = Sombrero::FastColor::HSVToRGB(fmod((currentTime * getTimeMult()) + mismatchSpeedOffset + seed, 1.0f), 1.0f, 1.0f);
                Sombrero::FastColor colorRight = Sombrero::FastColor::HSVToRGB(fmod((currentTime * getTimeMult()) + seed, 1.0f), 1.0f, 1.0f);
                light->ColorWasSet(leftColor ? colorLeft : colorRight);
            }
        }
    }
    else
    {
        Chroma::LightAPI::setAllLightingColorsSafe(true, Chroma::LightAPI::LSEData{gradientLeftColor, gradientRightColor, gradientLeftColor, gradientRightColor});
    }
}
void GradientController::rainbowGradientBackground() {
    setGradientColours(gradientLeftColor, gradientRightColor);
}
void GradientController::rainbowNotes() {
    Chroma::NoteAPI::setGlobalNoteColorSafe(gradientLeftColor, gradientRightColor);
}
void GradientController::rainbowWalls() {
    Chroma::ObstacleAPI::setAllObstacleColorSafe(gradientColor);
}
void GradientController::rainbowBombs() {
    Chroma::BombAPI::setGlobalBombColorSafe(gradientColor);
}
void GradientController::rainbowSabers() {
    Chroma::SaberAPI::setGlobalSaberColorSafe(GlobalNamespace::SaberType::SaberA, rainbowSaberColors[0]);
    Chroma::SaberAPI::setGlobalSaberColorSafe(GlobalNamespace::SaberType::SaberB, rainbowSaberColors[1]);
}

void GradientController::paletteTick() {
    rainbowSaberColors[0] = TechnicolourController::getLerpedFromVector(leftSaberPalette, currentTime + mismatchSpeedOffset);
    rainbowSaberColors[1] = TechnicolourController::getLerpedFromVector(rightSaberPalette, currentTime);
}
void GradientController::gradientTick() {
    rainbowSaberColors[0] = gradientLeftColor;
    rainbowSaberColors[1] = gradientRightColor;
}
void GradientController::setupWarmCold() {
    leftSaberPalette = TechnicolourController::getWarmPalette();
    rightSaberPalette = TechnicolourController::getColdPalette();
}
void GradientController::randomTick() {
    h += currentTime - lastTime;
    if (h > 1)
    {
        h = 0;
        randomCycleNext();
    }

    rainbowSaberColors[0] = Sombrero::FastColor::Lerp(randomCycleLeft[0], randomCycleLeft[1], h);
    rainbowSaberColors[1] = Sombrero::FastColor::Lerp(randomCycleRight[0], randomCycleRight[1], h);
    lastTime = currentTime;
}
void GradientController::randomCycleNext() {
    randomCycleLeft[0] = randomCycleLeft[1];
    randomCycleRight[0] = randomCycleRight[1];
    randomCycleLeft[1] = Sombrero::FastColor::HSVToRGB(TechnicolourController::randFloat(), 1.0f, 1.0f);
    if (match)
    {
        randomCycleRight[0] = randomCycleLeft[0];
        randomCycleRight[1] = randomCycleLeft[1];
    }
    else
    {
        randomCycleRight[1] = Sombrero::FastColor::HSVToRGB(TechnicolourController::randFloat(), 1.0f, 1.0f);
    }
}

Sombrero::FastColor randomColour() {
    return Sombrero::FastColor::HSVToRGB(TechnicolourController::randFloat(), 1.0f, 1.0f);
}

void GradientController::setupRandom() {
    randomCycleLeft[0] = randomColour();
    randomCycleLeft[1] = randomColour();
    randomCycleRight[0] = randomColour();
    randomCycleRight[1] = randomColour();
}