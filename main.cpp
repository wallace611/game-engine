#include "engine.h"
#include "object/cube.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "rendering/phong_drawer.h"
#include "rendering/skybox_drawer.h"
#include "input/input_mapper.h"
#include "object/light_object.h"

int main(int argc, char* argv[]) {
    EngineInit(&argc, argv);

    Cube* skybox = new Cube();
    skybox->SetGlobalPosition(glm::vec3(0.0f, 0.0f, -5.0f));
    skybox->SetGlobalScale(glm::vec3(1.0f, 2.0f, 1.0f));
    skybox->SetDrawer(new SkyboxDrawer("textures/skybox.png"));
    GetScene()->AddChild(skybox);

    GLuint envMapTexture = dynamic_cast<SkyboxDrawer*>(skybox->GetDrawer())->GetTextureID();

    Cube* cube = new Cube();
    cube->SetGlobalPosition(glm::vec3(-0.25f, -1.0f, -5.0f));
    cube->SetGlobalScale(glm::vec3(3.0f));
    cube->SetDrawer(new PhongDrawer("textures/wood.png", cube));
    PhongSettings woodSettings;
    woodSettings.ambientStrength = 0.3f;
    woodSettings.specularStrength = 0.5f;
    woodSettings.shininess = 16.0f;
    woodSettings.reflectivity = 0.2f;
    PhongDrawer* woodDrawer = dynamic_cast<PhongDrawer*>(cube->GetDrawer());
    woodDrawer->SetSettings(woodSettings);
    GetScene()->AddChild(cube);

    Cube* ironCube = new Cube();
    ironCube->SetGlobalPosition(glm::vec3(2.0f, -1.0f, -5.0f));
    ironCube->SetGlobalScale(glm::vec3(3.0f));
    ironCube->SetDrawer(new PhongDrawer("textures/iron.png", ironCube));
    PhongSettings ironSettings;
    ironSettings.ambientStrength = 0.1f;
    ironSettings.specularStrength = 2.0f;
    ironSettings.shininess = 64.0f;
    ironSettings.reflectivity = 0.5f;
    PhongDrawer* ironDrawer = dynamic_cast<PhongDrawer*>(ironCube->GetDrawer());
    ironDrawer->SetSettings(ironSettings);
    GetScene()->AddChild(ironCube);

    Cube* wierdCube = new Cube();
    wierdCube->SetGlobalPosition(glm::vec3(-2.5f, -1.0f, -5.0f));
    wierdCube->SetGlobalScale(glm::vec3(3.0f));
    wierdCube->SetDrawer(new PhongDrawer("textures/wierd.png", wierdCube));
    PhongSettings wierdSettings;
    wierdSettings.ambientStrength = 0.5f;
    wierdSettings.specularStrength = 0.5f;
    wierdSettings.shininess = 8.0f;
    wierdSettings.reflectivity = 0.8f;
    wierdSettings.scrollSpeed = glm::vec2(0.1f, 1.0f);
    PhongDrawer* wierdDrawer = dynamic_cast<PhongDrawer*>(wierdCube->GetDrawer());
    wierdDrawer->SetSettings(wierdSettings);
    GetScene()->AddChild(wierdCube);

    Cube* floor = new Cube();
    floor->SetGlobalPosition(glm::vec3(1.0f, -2.0f, 0.0f));
    floor->SetGlobalScale(glm::vec3(10.0f, 1.0f, 10.0f));
    floor->SetDrawer(new PhongDrawer("textures/grass.png", floor));
    PhongSettings floorSettings;
    floorSettings.ambientStrength = 0.2f;
    floorSettings.specularStrength = 0.05f;
    floorSettings.shininess = 8.0f;
    floorSettings.tiling = glm::vec2(20.0f, 20.0f);
    dynamic_cast<PhongDrawer*>(floor->GetDrawer())->SetSettings(floorSettings);
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