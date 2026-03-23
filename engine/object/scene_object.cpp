#include "scene_object.h"

#include <GL/glut.h>

#include "object/light_object.h"

Scene::Scene() : Object() {
    renderCamera = new Camera();
    lightSource = new LightObject();
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

Object *Scene::GetLightSource() {
    return lightSource;
}

int Scene::SetCamera(Camera *cam) {
    if (cam == nullptr) return -1;

    renderCamera = cam;
    return 0;
}
