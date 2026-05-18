#include "scene_initializing.h"
#include "rendering/model_drawer.h"
#include "rendering/drawer/phong_drawer.h"
#include "object/collision/aabb.h"
#include "object/collision/sphere.h"
#include "input/input_mapper.h"

#include "object/water_tank.h"
#include "object/water.h"
#include "object/ball.h"
#include "object/slab.h"
#include "object/boat.h"

#include <glm/gtc/random.hpp>

void InitScene() {
    Camera* cam = GetScene()->GetCamera();
    cam->SetLocalPosition(glm::vec3(0.0f, 4.0f, 4.0f));
    cam->SetLocalRotation(glm::vec3(0.0f, 0.0f, 0.0f));

    Object* root = new Object();
    root->name = "Root";
    GetScene()->AddChild(root);

    Object* actualTank = new Object();
    actualTank->name = "Tank";
    GetScene()->AddChild(actualTank, root);
    actualTank->SetDrawer(new PhongDrawer("model/tank.ply", "textures/tank.png", actualTank));
    actualTank->SetLocalPosition(glm::vec3(0.0f, 5.0f, -10.0f));
    actualTank->SetGlobalRotation(glm::vec3(-90.0f, 0.0f, 0.0f));

    Object* tankFloor = new Object();
    tankFloor->name = "TankFloor";
    GetScene()->AddChild(tankFloor, root);
    tankFloor->SetDrawer(new PhongDrawer("model/cube.ply", tankFloor));
    tankFloor->SetLocalPosition(glm::vec3(0.0f, 4.5f, -10.0f));
    tankFloor->SetLocalScale(glm::vec3(5.0f, 1.0f, 5.0f));
    tankFloor->SetDrawer(new PhongDrawer("model/cube.ply", tankFloor));

    WaterTank* waterTank = new WaterTank();
    GetScene()->AddChild(waterTank, root);

    Water* water = new Water(-5.0f, 5.0f, -5.0f, 5.0f);
    GetScene()->AddChild(water, waterTank);
    water->SetLocalPosition(glm::vec3(0.0f, 4.0f, 0.0f));
    
    Ball* ball = new Ball(water);
    GetScene()->AddChild(ball, root);
    ball->SetLocalPosition(glm::vec3(0.0f, 10.0f, 0.0f));
    ball->SetGlobalScale(glm::vec3(1.0f));

    Slab* slab = new Slab(water);
    GetScene()->AddChild(slab, root);
    slab->SetLocalPosition(glm::vec3(2.0f, 6.0f, 0.0f));

    Boat* boat = new Boat(water);
    GetScene()->AddChild(boat, root);
    boat->SetLocalPosition(glm::vec3(-2.0f, 4.0f, 0.0f));
}