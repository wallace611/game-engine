#pragma once

#include "object/object.h"

class Water : public Object {
protected:
    float minX, minZ, maxX, maxZ;
    float density = 1000.0f;
public:
    Water() : Water(-5.0f, 5.0f, -5.0f, 5.0f) {}
    Water(float minX, float maxX, float minZ, float maxZ);

    float GetYLevel() const { return GetGlobalPosition().y; }
    float GetDensity() const { return density; }
    void SetDensity(float newDensity) { density = newDensity; }
};