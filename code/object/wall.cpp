#include "wall.h"

#include "rendering/model_drawer.h"
#include "engine.h"

Wall::Wall(const glm::vec3 &position, const glm::vec3 &scale) {
    SetGlobalPosition(position);
    SetGlobalScale(scale);
}

void Wall::Ready() {
    SetDrawer(new ModelDrawer("model/cube.ply"));
    collider = new AABBCollider(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0.2f);
    collider->SetDebugMode(true);
    GetScene()->AddChild(collider, this);
}
