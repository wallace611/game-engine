#include "rendering/model_drawer.h"

#include <glm/glm.hpp>

class ColliderDrawer : public ModelDrawer {
private:
    glm::vec4 color;
    float faceAlpha;

    unsigned int faceIndexCount;
    unsigned int lineIndexCount;
    unsigned int lineOffset;
public:
    ColliderDrawer(const glm::vec4& color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), float faceAlpha = 0.1f);

    void UpdateAABB(const glm::vec3& minPt, const glm::vec3& maxPt);
    void UpdateSphere(const glm::vec3& center, float radius, int sectors = 36, int stacks = 18);

    virtual void Draw() override;
};