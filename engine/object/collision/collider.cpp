#include "collider.h"

#include "rendering/drawer/collider_drawer.h"

Collider::Collider(const glm::vec4 &shapeColor, float faceAlpha) : Object() {
    this->hitCallback = nullptr;
    this->isDebugMode = false;
    this->renderer = new ColliderDrawer(shapeColor, faceAlpha);
}

bool Collider::IsDebugModeEnabled() const { return this->isDebugMode; }

void Collider::SetDebugMode(bool debug) { this->isDebugMode = debug; }

void Collider::Render() {
    if (isDebugMode && renderer) {
        Object::Render();
    }
}
