#include "car.h"

#include "engine.h"
#include "rendering/drawer/phong_drawer.h"
#include "object/collision/sphere.h"

Car::Car(const glm::vec3 &mainColor) : ColorInterface(mainColor) {
    PhongDrawer* carDrawer = new PhongDrawer("model/911.ply", this);
    carDrawer->EnableCustomColor(true);
    carDrawer->SetCustomColor(mainColor);
    this->SetDrawer(carDrawer);
    for (int i = 0; i < 4; i++) {
        SphereCollider* front = new SphereCollider(glm::vec3(0.0f), 0.5f);
        front->SetGlobalPosition(glm::vec3(0.0f, 2.0f - i, 0.0f));
        GetScene()->AddChild(front, this);
    }   
}