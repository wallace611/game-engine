#include "water_tank.h"

#include "engine.h"
#include "object/collision/aabb.h"
#include "rendering/drawer/phong_drawer.h"

WaterTank::WaterTank() {
    name = "WaterTank";
    Object* floor = new Object();
    floor->name = "Floor";
    GetScene()->AddChild(floor, this);
    floor->SetLocalScale(glm::vec3(10.0f, 1.0f, 10.0f));
    floor->SetLocalPosition(glm::vec3(0.0f, -0.5f, 0.0f));
    floor->SetDrawer(new PhongDrawer("model/cube.ply", floor));

    AABBCollider* floorCollider = new AABBCollider(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f));
    floorCollider->SetDebugMode(true);
    GetScene()->AddChild(floorCollider, floor, false);

    Object* wall1 = new Object();
    wall1->name = "Wall1";
    GetScene()->AddChild(wall1, this);
    wall1->SetLocalScale(glm::vec3(10.0f, 5.0f, 1.0f));
    wall1->SetLocalPosition(glm::vec3(0.0f, 2.5f, -5.5f));
    wall1->SetDrawer(new PhongDrawer("model/cube.ply", wall1));

    AABBCollider* wall1Collider = new AABBCollider(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f));
    wall1Collider->SetDebugMode(true);
    GetScene()->AddChild(wall1Collider, wall1, false);

    Object* wall2 = new Object();
    wall2->name = "Wall2";
    GetScene()->AddChild(wall2, this);
    wall2->SetLocalScale(glm::vec3(10.0f, 5.0f, 1.0f));
    wall2->SetLocalPosition(glm::vec3(0.0f, 2.5f, 5.5f));
    wall2->SetDrawer(new PhongDrawer("model/cube.ply", wall2));

    AABBCollider* wall2Collider = new AABBCollider(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f));
    wall2Collider->SetDebugMode(true);
    GetScene()->AddChild(wall2Collider, wall2, false);

    Object* wall3 = new Object();
    wall3->name = "Wall3";
    GetScene()->AddChild(wall3, this);
    wall3->SetLocalScale(glm::vec3(1.0f, 5.0f, 10.0f));
    wall3->SetLocalPosition(glm::vec3(-5.5f, 2.5f, 0.0f));
    wall3->SetDrawer(new PhongDrawer("model/cube.ply", wall3));

    AABBCollider* wall3Collider = new AABBCollider(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f));
    wall3Collider->SetDebugMode(true);
    GetScene()->AddChild(wall3Collider, wall3, false);

    Object* wall4 = new Object();
    wall4->name = "Wall4";
    GetScene()->AddChild(wall4, this);
    wall4->SetLocalScale(glm::vec3(1.0f, 5.0f, 10.0f));
    wall4->SetLocalPosition(glm::vec3(5.5f, 2.5f, 0.0f));
    wall4->SetDrawer(new PhongDrawer("model/cube.ply", wall4));

    AABBCollider* wall4Collider = new AABBCollider(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f));
    wall4Collider->SetDebugMode(true);
    GetScene()->AddChild(wall4Collider, wall4, false);
}