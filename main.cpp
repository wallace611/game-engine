#include "engine.h"

#include "keybind.h"
#include "scene_initializing.h"
#include "camera_settings.h"

int main(int argc, char* argv[]) {
    EngineInit(&argc, argv);

    InitScene();

    InitKeybinds();

    EngineStartLoop();
}