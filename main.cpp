#include "engine.h"
#include "object/cube.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "rendering/phong_drawer.h"

int main(int argc, char* argv[]) {
    EngineInit(&argc, argv);

    Cube* cube = new Cube();
    cube->SetGlobalPosition(glm::vec3(0.0f, -2.0f, -5.0f));
    cube->SetDrawer(new PhongDrawer("shader/phong", "textures/wood.png", cube));
    GetScene()->AddChild(cube);

    Cube* floor = new Cube();
    floor->SetGlobalPosition(glm::vec3(1.0f, -2.0f, 0.0f));
    floor->SetGlobalScale(glm::vec3(10.0f, 1.0f, 10.0f));
    floor->SetDrawer(new PhongDrawer("shader/phong", "textures/wood.png", floor));
    GetScene()->AddChild(floor);

    Object* lightSrc = GetScene()->GetLightSource();
    lightSrc->SetGlobalPosition(glm::vec3(-2.0f, 2.0f, -3.0f));

    EngineStartLoop();
}