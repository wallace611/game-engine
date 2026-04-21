#include "man.h"

#include "engine.h"
#include "rendering/drawer/phong_drawer.h"
#include "object/collision/sphere.h"

Man::Man(const glm::vec3& mainColor) : Object(), ColorInterface(mainColor){
    PhongDrawer* manDrawer = new PhongDrawer("model/human.ply", this);
    manDrawer->EnableCustomColor(true);
    manDrawer->SetCustomColor(mainColor);
    this->SetDrawer(manDrawer);

    for (int i = 0; i < 4; i++) {
        SphereCollider* collider = new SphereCollider(glm::vec3(0.0f), 3.0f);
        collider->SetGlobalPosition(glm::vec3(0.0f, 0.0f, i * 5));
        GetScene()->AddChild(collider, this);
    }

}