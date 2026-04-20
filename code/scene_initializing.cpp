#include "scene_initializing.h"
#include "rendering/model_drawer.h"
#include "rendering/drawer/phong_drawer.h"
#include "object/collision/aabb.h"
#include "object/collision/sphere.h"
#include "object/wall.h"
#include "object/ball.h"
#include "input/input_mapper.h"

#include <glm/gtc/random.hpp>

void InitScene() {
    Camera* cam = GetScene()->GetCamera();
    cam->SetLocalPosition(glm::vec3(0.0f, 40.0f, 15.0f));
    cam->SetLocalRotation(glm::vec3(-60.0f, 0.0f, 0.0f));

    Object* root = new Object();
    GetScene()->AddChild(root);

    Wall* floor = new Wall(21, 1, 21, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    floor->SetGlobalPosition(glm::vec3(0.0f, -2.0f, 0.0f));
    GetScene()->AddChild(floor, root);

    Wall* wall1 = new Wall(glm::vec3(21.0f, 21.0f, 1.0f), glm::vec3(1.0f, 1.0f, 0.0f));
    wall1->SetGlobalPosition(glm::vec3(0.0f, 9.0f, 11.0f));
    GetScene()->AddChild(wall1, root);

    Wall* wall2 = new Wall(glm::vec3(21.0f, 21.0f, 1.0f), glm::vec3(1.0f, 1.0f, 0.0f));
    wall2->SetGlobalPosition(glm::vec3(0.0f, 9.0f, -11.0f));
    GetScene()->AddChild(wall2, root);

    Wall* wall3 = new Wall(glm::vec3(1.0f, 21.0f, 21.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    wall3->SetGlobalPosition(glm::vec3(11.0f, 9.0f, 0.0f));
    GetScene()->AddChild(wall3, root);

    Wall* wall4 = new Wall(glm::vec3(1.0f, 21.0f, 21.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    wall4->SetGlobalPosition(glm::vec3(-11.0f, 9.0f, 0.0f));
    GetScene()->AddChild(wall4, root);

    Object* car = new Object();
    car->SetGlobalPosition(glm::vec3(0.0f, -0.9f, 0.0f));
    car->SetGlobalRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
    PhongDrawer* carDrawer = new PhongDrawer("model/911.ply", car);
    carDrawer->EnableCustomColor(true);
    carDrawer->SetCustomColor(glm::vec3(1.0f, 0.0f, 0.0f));
    car->SetDrawer(carDrawer);
    GetScene()->AddChild(car, root);
    
}