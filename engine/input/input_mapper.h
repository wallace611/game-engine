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

class KeyMapperUnit {
public:
    bool isActive;

    std::function<void()> onPress;
    std::function<void()> onRelease;
    std::function<void()> onHold;

    KeyMapperUnit() : isActive(false) {}
};

static KeyMapperUnit inputMapper[256];

void InputMapperInit();
void InputMapperUpdate();

// Replaced raw function pointer with std::function<void()>
void InputRegisterKey(unsigned char key, action_t action, std::function<void()> callbackFunc);

// Private callbacks for GLUT
inline static void KeyboardPressedCallback(unsigned char key, int x, int y);
inline static void KeyboardReleaseCallback(unsigned char key, int x, int y);

class MouseMapperUnit {
public:
    bool isActive;
    std::function<void(int x, int y)> onPress;
    std::function<void(int x, int y)> onRelease;
    std::function<void(int x, int y)> onHold;

    MouseMapperUnit() : isActive(false) {}
};

static MouseMapperUnit mouseInputMapper[5];

struct MouseState {
    bool isPressed;
    int x;
    int y;
};

static MouseState mouseButtonCurrentStates[5]; // Track state for each mouse button
static MouseState mouseButtonLastStates[5];
extern bool allowMouseMotion;

#define MOUSE_LEFT_BTN GLUT_LEFT_BUTTON
#define MOUSE_RIGHT_BTN GLUT_RIGHT_BUTTON
#define MOUSE_MID_BTN GLUT_MIDDLE_BUTTON
#define MOUSE_MOTION 3

void InputRegisterMouse(int button, action_t action, std::function<void(int x, int y)> callbackFunc);

inline static void MouseActionCallback(int btn, int state, int x, int y);
inline static void MouseMovementCallback(int x, int y);