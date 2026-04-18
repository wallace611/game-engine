#include "engine.h"
#include "object/cube.h"
#include "input/input_mapper.h"
#include "object/light_object.h"
#include "utilities/happly.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
    EngineInit(&argc, argv);

    Cube* ironCube = new Cube();
    ironCube->SetGlobalPosition(glm::vec3(2.0f, -1.0f, -5.0f));
    ironCube->SetGlobalScale(glm::vec3(3.0f));
    ironCube->SetDrawer(new ModelDrawer());
   GetScene()->AddChild(ironCube);

    Cube* wierdCube = new Cube();
    wierdCube->SetGlobalPosition(glm::vec3(-2.5f, -1.0f, -5.0f));
    wierdCube->SetGlobalScale(glm::vec3(3.0f));
    wierdCube->SetDrawer(new ModelDrawer("textures/wierd.png"));
    GetScene()->AddChild(wierdCube);

    Cube* floor = new Cube();
    floor->SetGlobalPosition(glm::vec3(1.0f, -2.0f, 0.0f));
    floor->SetGlobalScale(glm::vec3(10.0f, 1.0f, 10.0f));
    floor->SetDrawer(new ModelDrawer("textures/grass.png"));
    GetScene()->AddChild(floor);

    LightObject* lightSrc = dynamic_cast<LightObject*>(GetScene()->GetLightSource());
    lightSrc->SetGlobalPosition(glm::vec3(0.0f, 0.5f, -1.0f));
    lightSrc->SetLightColor(glm::vec3(1.0f, 0.9f, 0.8f));

    Camera* cam = GetScene()->GetCamera();

    InputRegisterKey(' ', KEY_RELEASE, []() {
        std::cout << "Spacebar was pressed!" << std::endl;
    });

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

    InputRegisterKey('o', KEY_PRESS, [lightSrc]() {
        lightSrc->SetLightColor(glm::vec3(0.0f, 0.0f, 0.0f));
    });

    InputRegisterKey('p', KEY_PRESS, [lightSrc]() {
        lightSrc->SetLightColor(glm::vec3(1.0f, 0.9f, 0.8f));
    });

    InputRegisterKey('i', KEY_HOLD, [lightSrc]() {
        glm::vec3 currentPos = lightSrc->GetLocalPosition();
        lightSrc->SetLocalPosition(glm::vec3(currentPos.x, currentPos.y + 0.05f, currentPos.z));
    });

    InputRegisterKey('j', KEY_HOLD, [lightSrc]() {
        glm::vec3 currentPos = lightSrc->GetLocalPosition();
        lightSrc->SetLocalPosition(glm::vec3(currentPos.x - 0.05f, currentPos.y, currentPos.z));
    });

    InputRegisterKey('k', KEY_HOLD, [lightSrc]() {
        glm::vec3 currentPos = lightSrc->GetLocalPosition();
        lightSrc->SetLocalPosition(glm::vec3(currentPos.x, currentPos.y - 0.05f, currentPos.z));
    });

    InputRegisterKey('l', KEY_HOLD, [lightSrc]() {
        glm::vec3 currentPos = lightSrc->GetLocalPosition();
        lightSrc->SetLocalPosition(glm::vec3(currentPos.x + 0.05f, currentPos.y, currentPos.z));
    });

    InputRegisterKey('n', KEY_HOLD, [lightSrc]() {
        glm::vec3 currentColor = lightSrc->GetLightColor();
        lightSrc->SetLightColor(glm::vec3(currentColor.r < 0 ? 1.0f :currentColor.r - 0.01f, currentColor.g, currentColor.b));
    });

    InputRegisterKey('m', KEY_HOLD, [lightSrc]() {
        glm::vec3 currentColor = lightSrc->GetLightColor();
        lightSrc->SetLightColor(glm::vec3(currentColor.r, currentColor.g < 0 ? 1.0f : currentColor.g - 0.01f, currentColor.b));
    });

    InputRegisterKey(',', KEY_HOLD, [lightSrc]() {
        glm::vec3 currentColor = lightSrc->GetLightColor();
        lightSrc->SetLightColor(glm::vec3(currentColor.r, currentColor.g, currentColor.b < 0 ? 1.0f : currentColor.b - 0.01f));
    });

    EngineStartLoop();
}