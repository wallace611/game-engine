#include "engine.h"

#include "keybind.h"
#include "scene_initializing.h"

int main(int argc, char* argv[]) {
    EngineInit(&argc, argv);

    InitScene();

    InitKeybinds();

    EngineStartLoop();
}