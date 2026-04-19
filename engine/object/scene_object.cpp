#include "scene_object.h"

#include <GL/glut.h>

Scene::Scene() : Object() {
    renderCamera = new Camera();
}

void Scene::Ready() {
}

void Scene::Update(float deltatime) {
    renderCamera->Update(deltatime);
    Object::Update(deltatime);
}

void Scene::Render() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    renderCamera->Render();

    Object::Render();
}

Camera *Scene::GetCamera() {
    return renderCamera;
}

int Scene::SetCamera(Camera *cam) {
    if (cam == nullptr) return -1;

    renderCamera = cam;
    return 0;
}
