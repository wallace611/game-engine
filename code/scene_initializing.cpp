#include "scene_initializing.h"
#include "rendering/model_drawer.h"
#include "object/collision/aabb.h"
#include "object/collision/sphere.h"
#include "object/ball.h"

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

void CreateBall(const glm::vec3& position, float radius, Object* parent) {
    Object* ball = new Object();
    ball->SetGlobalPosition(position);
    ball->SetGlobalScale(glm::vec3(radius * 2.0f)); // Scale the unit sphere to the desired radius
    ball->SetTickFunction([](Object* self, float deltatime) {
        // Simple gravity effect
        static glm::vec3 velocity = glm::ballRand(10.0f); // Random initial velocity
        self->SetGlobalPosition(self->GetGlobalPosition() + velocity * deltatime);
    });
    GetScene()->AddChild(ball, parent);
    SphereCollider* ballCollider = new SphereCollider(glm::vec3(0.0f), radius, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 0.2f);
    ballCollider->SetDebugMode(true);
    GetScene()->AddChild(ballCollider, ball);
}

void InitScene() {
    Object* root = new Object();
    GetScene()->AddChild(root);

    CreateWall(glm::vec3(0.0f, -2.0f, 0.0f), glm::vec3(21.0f, 1.0f, 21.0f), root);
    CreateWall(glm::vec3(0.0f, 9.0f, -11.0f), glm::vec3(21.0f, 21.0f, 1.0f), root);
    CreateWall(glm::vec3(0.0f, 9.0f, 11.0f), glm::vec3(21.0f, 21.0f, 1.0f), root);
    CreateWall(glm::vec3(-11.0f, 9.0f, 0.0f), glm::vec3(1.0f, 21.0f, 21.0f), root);
    CreateWall(glm::vec3(11.0f, 9.0f, 0.0f), glm::vec3(1.0f, 21.0f, 21.0f), root);

    Ball* ball = new Ball(1.0f);
    GetScene()->AddChild(ball, root);
}