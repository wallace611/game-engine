#include <glad/glad.h>
#include "engine.h"
#include "input/input_mapper.h"
#include "utilities/happly.h"
#include "rendering/model_drawer.h"
#include "object/collision/aabb.h"
#include "object/collision/sphere.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
    EngineInit(&argc, argv);

    Object* man = new Object();
    man->SetGlobalPosition(glm::vec3(2.0f, -1.0f, -5.0f));
    man->SetGlobalScale(glm::vec3(0.3f));
    man->SetGlobalRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
    man->SetDrawer(new ModelDrawer("model/human.ply"));
    GetScene()->AddChild(man);

    Object* cm = new Object();
    cm->SetGlobalPosition(glm::vec3(-2.0f, 1.0f, 0.0f));
    cm->SetDrawer(new ModelDrawer("model/1m2.ply"));
    GetScene()->AddChild(cm);

    AABBCollider* aabb1 = new AABBCollider(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f));
    aabb1->SetGlobalPosition(glm::vec3(-3.0f, 3.5f, 0.0f));
    aabb1->SetDebugMode(true);
    aabb1->SetTickFunction([](Object* self, float deltatime) {
        static float timeElapsed = -3.0f;
        timeElapsed += deltatime;
        if (timeElapsed > 0) self->SetGlobalPosition(glm::vec3(-3.0f, 3.5f + sin(timeElapsed) * 2.0f, 0.0f)); 
    });
    GetScene()->AddChild(aabb1);

    SphereCollider* sphere1 = new SphereCollider(glm::vec3(0.0f), 0.5f);
    sphere1->SetGlobalPosition(glm::vec3(-3.0f, 2.0f, 0.0f));
    sphere1->SetDebugMode(true);
    sphere1->SetHitCallback([](Collider* self, Collider* from, const HitResult& hitResult) {
        self->SetGlobalPosition(self->GetGlobalPosition() + hitResult.hitNormal * hitResult.penetrationDepth); // Simple response: push the sphere out of collision
    });
    GetScene()->AddChild(sphere1);

    Object* car = new Object();
    car->SetGlobalPosition(glm::vec3(1.0f, -1.0, 1.0f));
    car->SetGlobalRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
    car->SetDrawer(new ModelDrawer("model/911.ply"));
    GetScene()->AddChild(car);

    Object* floor = new Object();
    floor->SetGlobalPosition(glm::vec3(1.0f, -2.0f, 0.0f));
    floor->SetGlobalScale(glm::vec3(100.0f, 1.0f, 100.0f));
    floor->SetDrawer(new ModelDrawer("model/cube.ply", "textures/grass.png"));
    GetScene()->AddChild(floor);

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
    InputRegisterKey('r', KEY_HOLD, [cam]() {
        cam->Move(glm::vec3(0.0f, 1.0f, 0.0f));
    });
    InputRegisterKey('f', KEY_HOLD, [cam]() {
        cam->Move(glm::vec3(0.0f, -1.0f, 0.0f));
    });

    EngineStartLoop();
}