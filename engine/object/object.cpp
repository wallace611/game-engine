#include "object.h"
#include "rendering/model_drawer.h"

#include <GL/glut.h>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

Object::Object() 
    : localPosition(0.0f), 
      localRotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)), // Identity quaternion
      localScale(1.0f), 
      localTransform(1.0f), 
      globalTransform(1.0f), 
      parent(nullptr), 
      renderer(nullptr) 
{
    UpdateTransforms();
}

Object::~Object() {
    if (renderer) delete renderer;
}

// ==========================================
// Core Matrix Update Logic
// ==========================================
void Object::UpdateTransforms() {
    glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), localPosition);
    glm::mat4 rotationMat = glm::toMat4(localRotation);
    glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), localScale);
    
    localTransform = translationMat * rotationMat * scaleMat;

    if (parent) {
        globalTransform = parent->globalTransform * localTransform;
    } else {
        globalTransform = localTransform;
    }

    for (Object* child : children) {
        child->UpdateTransforms();
    }
}

// ==========================================
// Local Getters & Setters
// ==========================================
glm::vec3 Object::GetLocalPosition() const { return localPosition; }
glm::quat Object::GetLocalRotationQuat() const { return localRotation; }
glm::vec3 Object::GetLocalScale() const { return localScale; }

glm::vec3 Object::GetLocalRotationEuler() const { 
    // Convert quaternion to euler radians, then to degrees
    return glm::degrees(glm::eulerAngles(localRotation)); 
}

void Object::SetLocalPosition(const glm::vec3& pos) {
    localPosition = pos;
    UpdateTransforms();
}

void Object::SetLocalRotation(const glm::quat& rot) {
    localRotation = rot;
    UpdateTransforms();
}

void Object::SetLocalRotation(const glm::vec3& eulerDegrees) {
    // Convert degrees to radians, then to quaternion
    localRotation = glm::quat(glm::radians(eulerDegrees));
    UpdateTransforms();
}

void Object::SetLocalScale(const glm::vec3& scale) {
    localScale = scale;
    UpdateTransforms();
}

// ==========================================
// Global Getters
// ==========================================
glm::vec3 Object::GetGlobalPosition() const {
    // Extract the position (translation) from the last column of the global matrix
    return glm::vec3(globalTransform[3]);
}

glm::quat Object::GetGlobalRotationQuat() const {
    if (parent) {
        // Global rotation is Parent's Global Rotation * Local Rotation
        return parent->GetGlobalRotationQuat() * localRotation;
    }
    return localRotation;
}

glm::vec3 Object::GetGlobalRotationEuler() const {
    return glm::degrees(glm::eulerAngles(GetGlobalRotationQuat()));
}

glm::vec3 Object::GetGlobalScale() const {
    if (parent) {
        return parent->GetGlobalScale() * localScale;
    }
    return localScale;
}

// ==========================================
// Global Setters (Advanced Math)
// ==========================================
void Object::SetGlobalPosition(const glm::vec3& pos) {
    if (parent) {
        // To set global position, we must multiply the target position 
        // by the inverse of the parent's global matrix to convert it back to local space.
        glm::mat4 parentInv = glm::inverse(parent->globalTransform);
        glm::vec4 newLocalPos = parentInv * glm::vec4(pos, 1.0f);
        SetLocalPosition(glm::vec3(newLocalPos));
    } else {
        SetLocalPosition(pos);
    }
}

void Object::SetGlobalRotation(const glm::quat& rot) {
    if (parent) {
        // Q_global = Q_parent * Q_local 
        // Therefore, Q_local = inverse(Q_parent) * Q_global
        glm::quat parentRotInv = glm::inverse(parent->GetGlobalRotationQuat());
        SetLocalRotation(parentRotInv * rot);
    } else {
        SetLocalRotation(rot);
    }
}

void Object::SetGlobalRotation(const glm::vec3& eulerDegrees) {
    SetGlobalRotation(glm::quat(glm::radians(eulerDegrees)));
}

void Object::SetGlobalScale(const glm::vec3& scale) {
    if (parent) {
        // Assuming no shear/skew matrices, global scale is simply parent * local
        glm::vec3 parentScale = parent->GetGlobalScale();
        SetLocalScale(scale / parentScale);
    } else {
        SetLocalScale(scale);
    }
}

// ==========================================
// Engine Hierarchy & Rendering
// ==========================================
glm::mat4 Object::GetGlobalMatrix() const {
    return globalTransform;
}

void Object::SetTickFunction(std::function<void(Object *, float)> tickFunc){
    tickFunction = tickFunc;
}

ModelDrawer *Object::GetDrawer() const { return renderer; }
void Object::SetDrawer(ModelDrawer *drawer) { renderer = drawer; }

void Object::Ready() {}

void Object::Update(float deltatime) {
    for (Object* child : children) {
        child->Update(deltatime);
    }

    if (tickFunction) tickFunction(this, deltatime);
}

void Object::Render() {
    glPushMatrix();
    
    glMultMatrixf(glm::value_ptr(localTransform));

    if (renderer) renderer->Draw(); // Pass matrices to shader if needed

    for (Object* child : children) {
        child->Render();
    }

    glPopMatrix();
}