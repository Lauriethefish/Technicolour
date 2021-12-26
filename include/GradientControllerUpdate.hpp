#pragma once

#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "GradientController.hpp"

DECLARE_CLASS_CODEGEN(Technicolor, GradientControllerUpdate, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_METHOD(void, Update);
public:
    GradientController gradientController;
);
