#include "water.h"

#include "engine.h"
#include "rendering/drawer/phong_drawer.h"

Water::Water(float minX, float maxX, float minZ, float maxZ) : minX(minX), maxX(maxX), minZ(minZ), maxZ(maxZ) {
    Object* waterSurface = new Object();
    GetScene()->AddChild(waterSurface, this);
    waterSurface->SetLocalPosition(glm::vec3((maxX + minX) / 2.0f, (maxZ + minZ) / 2.0f, 0.0f));
    waterSurface->SetLocalScale(glm::vec3(maxX - minX, maxZ - minZ, 1.0f));
    waterSurface->SetLocalRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
    waterSurface->SetDrawer(new PhongDrawer("model/plane.ply", "textures/water.png", waterSurface));
}
