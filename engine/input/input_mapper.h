#pragma once

#include <GL/glut.h>
#include <functional>

typedef unsigned char action_t;
#define KEY_PRESS   0b001
#define KEY_RELEASE 0b010
#define KEY_HOLD    0b100

// Using standard C++ arrays for state tracking
static bool keyInputCurrentState[256] = { false };
static bool keyInputLastState[256] = { false };

class MapperUnit {
public:
    bool isActive;

    std::function<void()> onPress;
    std::function<void()> onRelease;
    std::function<void()> onHold;

    MapperUnit() : isActive(false) {}
};

static MapperUnit inputMapper[256];

void InputMapperInit();
void InputMapperUpdate();

// Replaced raw function pointer with std::function<void()>
void InputRegisterKey(unsigned char key, action_t action, std::function<void()> callbackFunc);

// Private callbacks for GLUT
inline static void KeyboardPressedCallback(unsigned char key, int x, int y);
inline static void KeyboardReleaseCallback(unsigned char key, int x, int y);
inline static void MouseActionCallback(int btn, int state, int x, int y);
inline static void MouseMovementCallback(int x, int y);