#include "GradientControllerUpdate.hpp"

DEFINE_TYPE(Technicolour, GradientControllerUpdate);

void Technicolour::GradientControllerUpdate::Update() {
    gradientController.update();
}