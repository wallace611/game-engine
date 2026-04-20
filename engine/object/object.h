#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <functional>

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

    std::function<void(Object* self, float deltatime)> tickFunction;

    // Helper function to recalculate matrices when TRS changes
    void UpdateTransforms();
    
    friend class Scene; // Allow Scene to access protected members for hierarchy management
public:
    Object();
    virtual ~Object();

    // --- Local Getters ---
    virtual glm::vec3 GetLocalPosition() const;
    virtual glm::quat GetLocalRotationQuat() const;
    virtual glm::vec3 GetLocalRotationEuler() const; // Returns degrees
    virtual glm::vec3 GetLocalScale() const;

    // --- Local Setters ---
    virtual void SetLocalPosition(const glm::vec3& pos);
    virtual void SetLocalRotation(const glm::quat& rot);
    virtual void SetLocalRotation(const glm::vec3& eulerDegrees);
    virtual void SetLocalScale(const glm::vec3& scale);

    // --- Global Getters ---
    virtual glm::vec3 GetGlobalPosition() const;
    virtual glm::quat GetGlobalRotationQuat() const;
    virtual glm::vec3 GetGlobalRotationEuler() const; // Returns degrees
    virtual glm::vec3 GetGlobalScale() const;

    // --- Global Setters ---
    virtual void SetGlobalPosition(const glm::vec3& pos);
    virtual void SetGlobalRotation(const glm::quat& rot);
    virtual void SetGlobalRotation(const glm::vec3& eulerDegrees);
    virtual void SetGlobalScale(const glm::vec3& scale);

    // --- Engine Hierarchy & Rendering ---
    virtual glm::mat4 GetGlobalMatrix() const;

    Object* GetParent() const;

    void SetTickFunction(std::function<void(Object*, float)> tickFunc);
    ModelDrawer* GetDrawer() const;
    void SetDrawer(ModelDrawer* drawer);

    virtual void Ready();
    virtual void Update(float deltatime);
    virtual void Render();
};