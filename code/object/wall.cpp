#include "wall.h"

#include "rendering/model_drawer.h"
#include "rendering/drawer/phong_drawer.h"
#include "object/collision/aabb.h"
#include "engine.h"


Wall::Wall(int x, int y, int z, const glm::vec3& color1, const glm::vec3& color2) : ColorInterface(color1) {
    bool flag = true;
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            for (int k = 0; k < z; k++) {
                Object* cube = new Object();
                cube->SetLocalPosition(glm::vec3(i - x / 2, j - y / 2, k - z / 2));
                PhongDrawer* drawer = new PhongDrawer("model/cube.ply", cube);
                drawer->EnableCustomColor(true);
                drawer->SetCustomColor(flag ? color1 : color2);
                flag = !flag;
                cube->SetDrawer(drawer);
                GetScene()->AddChild(cube, this);
            }
        }
    }
    AABBCollider* collider = new AABBCollider(glm::vec3(-0.5f), glm::vec3(0.5f));
    collider->SetLocalScale(glm::vec3(x, y, z));
    GetScene()->AddChild(collider, this, false);
}

Wall::Wall(glm::vec3 &size, glm::vec3 &mainColor) : ColorInterface(mainColor) {
    Object* cube = new Object();
    cube->SetLocalScale(size);
    PhongDrawer* drawer = new PhongDrawer("model/cube.ply", cube);
    drawer->EnableCustomColor(true);
    drawer->SetCustomColor(mainColor);
    cube->SetDrawer(drawer);
    GetScene()->AddChild(cube, this);
    AABBCollider* collider = new AABBCollider(glm::vec3(-0.5f), glm::vec3(0.5f));
    collider->SetLocalScale(size);
    GetScene()->AddChild(collider, this, false);
}

void Wall::Ready() {
}
