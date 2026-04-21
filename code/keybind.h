#pragma once

#include "input/input_mapper.h"
#include "object/scene_object.h"
#include "engine/engine.h"
#include "object/ball.h"

#include "object/scene/collision_optimized_scene.h"

#include "camera_settings.h"

#include <iostream>

void InitKeybinds() {
    Camera* cam = GetScene()->GetCamera();

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
    InputRegisterKey('t', KEY_PRESS, NextCamera);
    InputRegisterKey('p', KEY_PRESS, []() {
        PauseGame();
    });
    InputRegisterMouse(MOUSE_MOTION, 0, [cam](int x, int y) {
        cam->Rotate((float) -y / 5, (float) -x / 5);
    });
    InputRegisterMouse(MOUSE_LEFT_BTN, KEY_PRESS, [](int x, int y) {
        Camera* cam = GetScene()->GetCamera();
        Ball* ball = new Ball(cam->GetGlobalPosition(), cam->GetCameraFront() * 10.0f);
        GetScene()->AddChild(ball);
    });
    InputRegisterKey('g', KEY_PRESS, []() {
    });
}