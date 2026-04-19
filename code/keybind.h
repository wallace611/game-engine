#pragma once

#include "input/input_mapper.h"
#include "object/scene_object.h"
#include "engine/engine.h"

void InitKeybinds() {
    Camera* cam = GetScene()->GetCamera();

    InputRegisterKey('q', KEY_HOLD, [cam]() {
        cam->Rotate(0.0f, 1.0f);
    });

    InputRegisterKey('e', KEY_HOLD, [cam]() {
        cam->Rotate(0.0f, -1.0f);
    });

    InputRegisterKey('w', KEY_HOLD, [cam]() {
        cam->Move(glm::vec3(0.0f, 0.0f, 1.0f));
    });

    InputRegisterKey('a', KEY_HOLD, [cam]() {
        cam->Move(glm::vec3(-1.0f, 0.0f, 0.0f));
    });
    InputRegisterKey('s', KEY_HOLD, [cam]() {
        cam->Move(glm::vec3(0.0f, 0.0f, -1.0f));
    });
    InputRegisterKey('d', KEY_HOLD, [cam]() {
        cam->Move(glm::vec3(1.0f, 0.0f, 0.0f));
    });
    InputRegisterKey('r', KEY_HOLD, [cam]() {
        cam->Move(glm::vec3(0.0f, 1.0f, 0.0f));
    });
    InputRegisterKey('f', KEY_HOLD, [cam]() {
        cam->Move(glm::vec3(0.0f, -1.0f, 0.0f));
    });
    InputRegisterKey('v', KEY_PRESS, [cam]() {
        cam->SetProjectionMode(!cam->GetProjectionMode());
    });
    InputRegisterKey('t', KEY_HOLD, [cam]() {
        cam->Rotate(1.0f, 0.0f);
    });
    InputRegisterKey('g', KEY_HOLD, [cam]() {
        cam->Rotate(-1.0f, 0.0f);
    });
}