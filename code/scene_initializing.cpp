#include "scene_initializing.h"
#include "rendering/model_drawer.h"
#include "object/collision/aabb.h"
#include "object/collision/sphere.h"
#include "object/wall.h"
#include "object/ball.h"
#include "input/input_mapper.h"

#include <glm/gtc/random.hpp>

void CreateWall(const glm::vec3& position, const glm::vec3& scale, Object* parent) {
    Object* wall = new Object();
    wall->SetGlobalPosition(position);
    wall->SetGlobalScale(scale);
    wall->SetDrawer(new ModelDrawer("model/cube.ply"));
    GetScene()->AddChild(wall, parent);
    AABBCollider* wallCollider = new AABBCollider(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0.2f);
    wallCollider->SetDebugMode(true);
    GetScene()->AddChild(wallCollider, wall);
}

void InitScene() {
    Camera* cam = GetScene()->GetCamera();
    cam->SetLocalPosition(glm::vec3(0.0f, 40.0f, 15.0f));
    cam->SetLocalRotation(glm::vec3(-60.0f, 0.0f, 0.0f));

    Object* root = new Object();
    GetScene()->AddChild(root);

    Wall* wall1 = new Wall(glm::vec3(0.0f, -2.0f, 0.0f), glm::vec3(21.0f, 1.0f, 21.0f));
    GetScene()->AddChild(wall1, root);

    InputRegisterKey('b', KEY_PRESS, []() {
        Camera* cam = GetScene()->GetCamera();
        Ball* ball = new Ball(cam->GetGlobalPosition(), cam->GetCameraFront() * 10.0f);
        GetScene()->AddChild(ball);
    });
}