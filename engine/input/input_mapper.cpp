#include "input_mapper.h"
#include "../rendering/imgui_layer.h"

bool allowMouseMotion;

void InputMapperInit() {
    // Bind GLUT functions to our internal static callbacks
    glutKeyboardFunc(KeyboardPressedCallback);
    glutKeyboardUpFunc(KeyboardReleaseCallback);
    glutMouseFunc(MouseActionCallback);
    glutPassiveMotionFunc(MouseMovementCallback);
    allowMouseMotion = false;
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
    int i;
    for (i = 0; i < 256; i++) {
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

    for (i = 0; i < 3; i++) {
        // Skip inactive mouse buttons to save CPU cycles
        if (!mouseInputMapper[i].isActive) continue;

        bool isPressed = mouseButtonCurrentStates[i].isPressed;
        bool wasPressed = mouseButtonLastStates[i].isPressed;
        int x = mouseButtonCurrentStates[i].x;
        int y = mouseButtonCurrentStates[i].y;

        // 1. Check for MOUSE_PRESS (Triggered exactly once when the button goes down)
        if (isPressed && !wasPressed && mouseInputMapper[i].onPress) {
            mouseInputMapper[i].onPress(x, y);
        }

        // 2. Check for MOUSE_RELEASE (Triggered exactly once when the button comes up)
        if (!isPressed && wasPressed && mouseInputMapper[i].onRelease) {
            mouseInputMapper[i].onRelease(x, y);
        }

        // 3. Check for MOUSE_HOLD (Triggered continuously every frame while held)
        if (isPressed && mouseInputMapper[i].onHold) {
            mouseInputMapper[i].onHold(x, y);
        }

        // Update the last state for the next frame calculation
        mouseButtonLastStates[i] = mouseButtonCurrentStates[i];
    }
    i = MOUSE_MOTION;
    if (mouseInputMapper[i].isActive) {
        if (allowMouseMotion && !ImGuiWantsMouse()) {
            int x = mouseButtonCurrentStates[i].x - mouseButtonLastStates[i].x;
            int y = mouseButtonCurrentStates[i].y - mouseButtonLastStates[i].y;

            mouseInputMapper[i].onHold(x, y);
        }
        mouseButtonLastStates[i] = mouseButtonCurrentStates[i];
    }
}

inline void KeyboardPressedCallback(unsigned char key, int x, int y) {
    ImGuiLayerKeyDown(key);
    if (!ImGuiWantsKeyboard()) keyInputCurrentState[key] = true;
}

inline void KeyboardReleaseCallback(unsigned char key, int x, int y) {
    ImGuiLayerKeyUp(key);
    keyInputCurrentState[key] = false;
}

void InputRegisterMouse(int button, action_t action,
                        std::function<void(int x, int y)> callbackFunc) {
    mouseInputMapper[button].isActive = true;

    if (action & KEY_PRESS) {
        mouseInputMapper[button].onPress = callbackFunc;
    }
    if (action & KEY_RELEASE) {
        mouseInputMapper[button].onRelease = callbackFunc;
    }
    if ((action & KEY_HOLD) || button == MOUSE_MOTION) {
        mouseInputMapper[button].onHold = callbackFunc;
    }
}

inline void MouseActionCallback(int btn, int state, int x, int y) {
    ImGuiLayerAddMousePos((float)x, (float)y);
    if (btn >= 0 && btn < 3) ImGuiLayerAddMouseButton(btn, state == GLUT_DOWN);
    mouseButtonCurrentStates[btn].x = x;
    mouseButtonCurrentStates[btn].y = y;
    mouseButtonCurrentStates[btn].isPressed = (state == GLUT_DOWN);
}

inline void MouseMovementCallback(int x, int y) {
    ImGuiLayerAddMousePos((float)x, (float)y);
    mouseButtonCurrentStates[MOUSE_MOTION].x = x;
    mouseButtonCurrentStates[MOUSE_MOTION].y = y;
    mouseButtonCurrentStates[MOUSE_MOTION].isPressed = true;
}