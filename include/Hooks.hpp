#pragma once
#include "sombrero/shared/ColorUtils.hpp"

// Keys are lightsID, values are BeatmapEventData values
std::unordered_map<int, int>& getLatestLightEventDataValues();

// Sets the current gradient background colour
void setGradientColours(const Sombrero::FastColor& colorLeft, const Sombrero::FastColor& colorRight);

// Installs all technicolor hooks
void installHooks();
