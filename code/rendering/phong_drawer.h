#pragma once

#include "rendering/model_drawer.h"
#include "object/object.h"

class PhongDrawer : public ModelDrawer {
private:
    Object* owner;
public:
    PhongDrawer(const std::string& shader_path, const std::string& texture_path, Object* owner);
    virtual void Draw() override;
};