#pragma once

#include "object/object.h"
#include "rendering/model_drawer.h"

class Cube : public Object {
private:
    ModelDrawer* model;

public:
    Cube();
};