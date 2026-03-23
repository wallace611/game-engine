#pragma once

#include "rendering/model_drawer.h"

class SkyboxDrawer : public ModelDrawer {
public:
    SkyboxDrawer(const std::string& texture_path);
    virtual void Draw();
};