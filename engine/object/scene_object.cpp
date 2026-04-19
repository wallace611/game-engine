#include "scene_object.h"

#include <GL/glut.h>
#include <iostream>

#include "object/collision/collider.h"

Scene::Scene() : Object() {
    renderCamera = new Camera();
}

void Scene::Ready() {
}

void Scene::Update(float deltatime) {
    renderCamera->Update(deltatime);
    Object::Update(deltatime);
}

void Scene::CollisionCheck() {
    // Check collisions between all pairs of colliders in the scene
    for (size_t i = 0; i < colliders.size(); ++i) {
        for (size_t j = i + 1; j < colliders.size(); ++j) {
            HitResult hitResult;
            if (colliders[i]->CollideWith(colliders[j], hitResult)) {
                // Handle collision response here if needed
                if (colliders[i]->GetHitCallback()) {
                    colliders[i]->GetHitCallback()(colliders[i], colliders[j], hitResult);
                }
                if (colliders[j]->GetHitCallback()) {
                    hitResult.hitNormal = -hitResult.hitNormal; // Invert normal for the other collider's perspective
                    colliders[j]->GetHitCallback()(colliders[j], colliders[i], hitResult);
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

void Scene::AddChild(Object *child, Object *parent) {
    if (!parent) parent = this; // If no parent specified, add to root of scene
    parent->children.push_back(child);
    child->parent = parent;
    child->UpdateTransforms();
    child->Ready();

    // If the child is a collider, add it to the colliders list
    Collider* colliderChild = dynamic_cast<Collider*>(child);
    if (colliderChild) {
        colliders.push_back(colliderChild);
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
