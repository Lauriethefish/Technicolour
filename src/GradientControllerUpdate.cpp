#include "GradientControllerUpdate.hpp"

DEFINE_TYPE(Technicolor, GradientControllerUpdate);

void Technicolor::GradientControllerUpdate::Update() {
    gradientController.update();
}
