#include "input_mapper.h"

void InputMapperInit() {
    // Bind GLUT functions to our internal static callbacks
    glutKeyboardFunc(KeyboardPressedCallback);
    glutKeyboardUpFunc(KeyboardReleaseCallback);
    glutMouseFunc(MouseActionCallback);
    glutPassiveMotionFunc(MouseMovementCallback);
}

void InputRegisterKey(unsigned char key, action_t action, std::function<void()> callbackFunc) {
    inputMapper[key].isActive = true;

    // Assign the callback to the corresponding action state
    if (action & KEY_PRESS) {
        inputMapper[key].onPress = callbackFunc;
    }
    if (action & KEY_RELEASE) {
        inputMapper[key].onRelease = callbackFunc;
    }
    if (action & KEY_HOLD) {
        inputMapper[key].onHold = callbackFunc;
    }
}

void InputMapperUpdate() {
    for (int i = 0; i < 256; i++) {
        // Skip inactive keys to save CPU cycles
        if (!inputMapper[i].isActive) continue;

        bool isPressed = keyInputCurrentState[i];
        bool wasPressed = keyInputLastState[i];

        // 1. Check for KEY_PRESS (Triggered exactly once when the key goes down)
        if (isPressed && !wasPressed && inputMapper[i].onPress) {
            inputMapper[i].onPress();
        }

        // 2. Check for KEY_RELEASE (Triggered exactly once when the key comes up)
        if (!isPressed && wasPressed && inputMapper[i].onRelease) {
            inputMapper[i].onRelease();
        }

        // 3. Check for KEY_HOLD (Triggered continuously every frame while held)
        if (isPressed && inputMapper[i].onHold) {
            inputMapper[i].onHold();
        }

        // Update the last state for the next frame calculation
        keyInputLastState[i] = isPressed;
    }
}

inline void KeyboardPressedCallback(unsigned char key, int x, int y) {
    keyInputCurrentState[key] = true;
}

inline void KeyboardReleaseCallback(unsigned char key, int x, int y) {
    keyInputCurrentState[key] = false;
}

inline void MouseActionCallback(int btn, int state, int x, int y) {
    // To be implemented: Map GLUT mouse buttons to your new C++ mouse state arrays
}

inline void MouseMovementCallback(int x, int y) {
    // To be implemented: Update mouse X/Y deltas
}