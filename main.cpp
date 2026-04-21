#include "engine.h"

#include "keybind.h"
#include "scene_initializing.h"
#include "camera_settings.h"
#include "object/scene/collision_optimized_scene.h"

int main(int argc, char* argv[]) {
    EngineInit(&argc, argv);
    Scene* tmp = GetScene();
    delete tmp;
    SetScene(new CollisionOptimizedScene());

    InitScene();

    InitKeybinds();

    EngineStartLoop();
}