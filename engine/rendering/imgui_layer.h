#pragma once

class Scene;

// Lifecycle
void ImGuiLayerInit();
void ImGuiLayerBegin(int width, int height, float deltaTime);
void ImGuiLayerEnd();
void ImGuiLayerShutdown();

// Input forwarding (called from input_mapper callbacks)
void ImGuiLayerAddChar(unsigned char c);
void ImGuiLayerAddMousePos(float x, float y);
void ImGuiLayerAddMouseButton(int button, bool down);

bool ImGuiWantsMouse();
bool ImGuiWantsKeyboard();

// Panel
void DrawSceneInspector(Scene* scene);
