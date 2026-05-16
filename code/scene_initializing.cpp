#include "scene_initializing.h"
#include "rendering/model_drawer.h"
#include "rendering/drawer/phong_drawer.h"
#include "object/collision/aabb.h"
#include "object/collision/sphere.h"
#include "object/wall.h"
#include "object/ball.h"
#include "object/car.h"
#include "object/man.h"
#include "input/input_mapper.h"

#include <glm/gtc/random.hpp>

void InitScene() {
    Camera* cam = GetScene()->GetCamera();
    cam->SetLocalPosition(glm::vec3(0.0f, 40.0f, 15.0f));
    cam->SetLocalRotation(glm::vec3(-60.0f, 0.0f, 0.0f));

    Object* root = new Object();
    GetScene()->AddChild(root);

    Wall* floor = new Wall(21, 1, 21, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    GetScene()->AddChild(floor, root);
    floor->SetGlobalPosition(glm::vec3(0.0f, -2.0f, 0.0f));

    Wall* wall1 = new Wall(glm::vec3(21.0f, 21.0f, 1.0f), glm::vec3(1.0f, 1.0f, 0.0f));
    GetScene()->AddChild(wall1, root);
    wall1->SetGlobalPosition(glm::vec3(0.0f, 9.0f, 11.0f));

    Wall* wall2 = new Wall(glm::vec3(21.0f, 21.0f, 1.0f), glm::vec3(1.0f, 1.0f, 0.0f));
    GetScene()->AddChild(wall2, root);
    wall2->SetGlobalPosition(glm::vec3(0.0f, 9.0f, -11.0f));

    Wall* wall3 = new Wall(glm::vec3(1.0f, 21.0f, 21.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    GetScene()->AddChild(wall3, root);
    wall3->SetGlobalPosition(glm::vec3(11.0f, 9.0f, 0.0f));

    Wall* wall4 = new Wall(glm::vec3(1.0f, 21.0f, 21.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    GetScene()->AddChild(wall4, root);
    wall4->SetGlobalPosition(glm::vec3(-11.0f, 9.0f, 0.0f));

    AABBCollider* top = new AABBCollider(glm::vec3(-0.5), glm::vec3(0.5));
    top->SetDebugMode(true);
    GetScene()->AddChild(top, root);
    top->SetGlobalPosition(glm::vec3(0.0f, 22.0f, 0.0f));
    top->SetGlobalScale(glm::vec3(21.0f, 1.0f, 21.0f));

    Car* car = new Car(glm::vec3(1.0f, 0.0f, 0.0f));
    GetScene()->AddChild(car, root);
    car->SetGlobalPosition(glm::vec3(0.0f, -0.9f, 3.0f));
    car->SetGlobalRotation(glm::vec3(-90.0f, 30.0f, 0.0f));

    Car* car2 = new Car(glm::vec3(0.5f, 1.0f, 0.1f));
    GetScene()->AddChild(car2, car);
    car2->SetGlobalPosition(glm::vec3(0.0f, -0.9f, -6.0f));
    car2->SetGlobalRotation(glm::vec3(-90.0f, -45.0f, 0.0f));

    Man* man1 = new Man(glm::vec3(0.0f, 0.0f, 1.0f));
    GetScene()->AddChild(man1, root);
    man1->SetGlobalPosition(glm::vec3(-8.0f, -1.5f, -8.0f));
    man1->SetGlobalScale(glm::vec3(0.3));
    man1->SetGlobalRotation(glm::vec3(-90.0f, 30.0f, 0.0f));


    Man* man2 = new Man(glm::vec3(1.0f, 0.0f, 0.5f));
    GetScene()->AddChild(man2, root);
    man2->SetGlobalPosition(glm::vec3(8.0f, -1.5f, 8.0f));
    man2->SetGlobalScale(glm::vec3(0.4));
    man2->SetGlobalRotation(glm::vec3(-90.0f, 210.0f, 0.0f));

}