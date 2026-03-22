#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

class ModelDrawer;

class Object {
protected:
    glm::vec3 localPosition;
    glm::quat localRotation; 
    glm::vec3 localScale;

    // Cached matrices for rendering and global calculations
    glm::mat4 localTransform;
    glm::mat4 globalTransform;

    Object* parent;
    std::vector<Object*> children;
    ModelDrawer* renderer;

    // Helper function to recalculate matrices when TRS changes
    void UpdateTransforms();

public:
    Object();
    virtual ~Object();

    // --- Local Getters ---
    glm::vec3 GetLocalPosition() const;
    glm::quat GetLocalRotationQuat() const;
    glm::vec3 GetLocalRotationEuler() const; // Returns degrees
    glm::vec3 GetLocalScale() const;

    // --- Local Setters ---
    void SetLocalPosition(const glm::vec3& pos);
    void SetLocalRotation(const glm::quat& rot);
    void SetLocalRotation(const glm::vec3& eulerDegrees);
    void SetLocalScale(const glm::vec3& scale);

    // --- Global Getters ---
    glm::vec3 GetGlobalPosition() const;
    glm::quat GetGlobalRotationQuat() const;
    glm::vec3 GetGlobalRotationEuler() const; // Returns degrees
    glm::vec3 GetGlobalScale() const;

    // --- Global Setters ---
    void SetGlobalPosition(const glm::vec3& pos);
    void SetGlobalRotation(const glm::quat& rot);
    void SetGlobalRotation(const glm::vec3& eulerDegrees);
    void SetGlobalScale(const glm::vec3& scale);

    // --- Engine Hierarchy & Rendering ---
    glm::mat4 GetGlobalMatrix() const;
    
    void AddChild(Object* child);
    ModelDrawer* GetDrawer();
    void SetDrawer(ModelDrawer* drawer);

    virtual void Ready();
    virtual void Update(float deltatime);
    virtual void Render();
};