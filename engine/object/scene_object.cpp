#include "scene_object.h"

#include <GL/glut.h>
#include <iostream>

#include "object/collision/collider.h"

Scene::Scene() : Object() {
    renderCamera = new Camera();
    light = new Object();
    light->SetGlobalPosition(glm::vec3(0.0f, 22.0f, 0.0f));
}

void Scene::Ready() {
}

void Scene::Update(float deltatime) {
    renderCamera->Update(deltatime);
    Object::Update(deltatime);
}

void Scene::CollisionCheck() {
    // Check collisions between all pairs of colliders in the scene
    for (size_t i = 0; i < dynamicCollider.size(); ++i) {
        for (size_t j = i + 1; j < dynamicCollider.size(); ++j) {
            HitResult hitResult;
            if (dynamicCollider[i]->CollideWith(dynamicCollider[j], hitResult)) {
                // Handle collision response here if needed
                if (dynamicCollider[i]->GetHitCallback()) {
                    dynamicCollider[i]->GetHitCallback()(dynamicCollider[i], dynamicCollider[j], hitResult);
                }
                if (dynamicCollider[j]->GetHitCallback()) {
                    hitResult.hitNormal = -hitResult.hitNormal; // Invert normal for the other collider's perspective
                    dynamicCollider[j]->GetHitCallback()(dynamicCollider[j], dynamicCollider[i], hitResult);
                }
            }
        }
    }
    for (size_t i = 0; i < staticCollider.size(); i++) {
        for (size_t j = 0; j < dynamicCollider.size(); j++) {
            HitResult hitResult;
            if (staticCollider[i]->CollideWith(dynamicCollider[j], hitResult)) {
                if (staticCollider[i]->GetHitCallback()) {
                    staticCollider[i]->GetHitCallback()(staticCollider[i], dynamicCollider[j], hitResult);
                }
                if (dynamicCollider[j]->GetHitCallback()) {
                    hitResult.hitNormal = -hitResult.hitNormal;
                    dynamicCollider[j]->GetHitCallback()(dynamicCollider[j], staticCollider[i], hitResult);
                }
            }
        }
    }
}

void Scene::Render() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    renderCamera->Render();

    Object::Render();
}

void Scene::AddChild(Object *child, Object *parent, bool isDynamic) {
    if (!parent) parent = this; // If no parent specified, add to root of scene
    parent->children.push_back(child);
    child->parent = parent;
    child->UpdateTransforms();
    child->Ready();

    // If the child is a collider, add it to the colliders list
    Collider* colliderChild = dynamic_cast<Collider*>(child);
    if (colliderChild) {
        if (isDynamic)
            dynamicCollider.push_back(colliderChild);
        else
            staticCollider.push_back(colliderChild);
    }
}

Camera *Scene::GetCamera() {
    return renderCamera;
}

int Scene::SetCamera(Camera *cam) {
    if (cam == nullptr) return -1;

    renderCamera = cam;
    return 0;
}
