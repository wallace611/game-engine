#include "imgui_layer.h"

#include <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "object/scene_object.h"
#include "object/object.h"
#include "object/boat.h"

static Object* selectedObject = nullptr;
static glm::vec3 ui_applyForcePosition = glm::vec3(0.0f);
static glm::vec3 ui_applyForce = glm::vec3(0.0f);

// --- Scene tree ---

static void DrawObjectTree(Object* obj) {
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    if (obj->GetChildren().empty()) flags |= ImGuiTreeNodeFlags_Leaf;
    if (selectedObject == obj)      flags |= ImGuiTreeNodeFlags_Selected;

    bool open = ImGui::TreeNodeEx((void*)obj, flags, "%s", obj->name.c_str());

    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
        selectedObject = obj;

    if (open) {
        for (Object* child : obj->GetChildren())
            DrawObjectTree(child);
        ImGui::TreePop();
    }
}

// --- Properties panel ---

static void DrawPropertiesPanel(Object* obj) {
    ImGui::Text("[ %s ]", obj->name.c_str());
    ImGui::Separator();
    ImGui::Spacing();

    glm::vec3 pos = obj->GetLocalPosition();
    if (ImGui::DragFloat3("Local Position", glm::value_ptr(pos), 0.05f))
        obj->SetLocalPosition(pos);

    glm::vec3 rot = obj->GetLocalRotationEuler();
    if (ImGui::DragFloat3("Local Rotation", glm::value_ptr(rot), 0.5f))
        obj->SetLocalRotation(rot);

    glm::vec3 scale = obj->GetLocalScale();
    if (ImGui::DragFloat3("Local Scale", glm::value_ptr(scale), 0.05f, 0.001f, 100.0f))
        obj->SetLocalScale(scale);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::TextDisabled("Global");
    ImGui::Spacing();

    glm::vec3 gpos = obj->GetGlobalPosition();
    if (ImGui::DragFloat3("Global Position", glm::value_ptr(gpos), 0.05f))
        obj->SetGlobalPosition(gpos);

    glm::vec3 grot = obj->GetGlobalRotationEuler();
    if (ImGui::DragFloat3("Global Rotation", glm::value_ptr(grot), 0.5f))
        obj->SetGlobalRotation(grot);

    glm::vec3 gscale = obj->GetGlobalScale();
    if (ImGui::DragFloat3("Global Scale", glm::value_ptr(gscale), 0.05f, 0.001f, 100.0f))
        obj->SetGlobalScale(gscale);

    // Boat-specific controls
    if (Boat* boat = dynamic_cast<Boat*>(obj)) {
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Boat Controls");
        if (ImGui::DragFloat3("Force Position (world)", glm::value_ptr(ui_applyForcePosition), 0.1f)) {}
        if (ImGui::DragFloat3("Force", glm::value_ptr(ui_applyForce), 0.1f)) {}
        if (ImGui::Button("Apply Force")) {
            boat->AddForce(ui_applyForcePosition, ui_applyForce);
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Metal Cube");
        
        // Display total mass and COM info
        float totalMass = boat->GetTotalMass();
        glm::vec3 comLocal = boat->GetCOMLocal();
        ImGui::Text("Total Mass: %.2f kg", totalMass);
        ImGui::Text("COM (local): (%.3f, %.3f, %.3f)", comLocal.x, comLocal.y, comLocal.z);
        
        // Control metal cube position and mass
        Object* metalCube = boat->GetMetalCube();
        if (metalCube) {
            glm::vec3 metalPos = metalCube->GetLocalPosition();
            if (ImGui::DragFloat3("Metal Cube Position (local)", glm::value_ptr(metalPos), 0.05f, -1.0f, 1.0f)) {
                metalCube->SetLocalPosition(metalPos);
            }
        }
        
        // Metal cube mass
        float metalMass = boat->GetMetalCubeMass();
        if (ImGui::DragFloat("Metal Cube Mass (kg)", &metalMass, 1.0f, 0.1f, 10000.0f)) {
            boat->SetMetalCubeMass(metalMass);
        }
    }
}

// --- Public API ---

void ImGuiLayerInit() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplOpenGL3_Init("#version 330");
}

void ImGuiLayerBegin(int width, int height, float deltaTime) {
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)width, (float)height);
    io.DeltaTime   = (deltaTime > 0.0f) ? deltaTime : (1.0f / 60.0f);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayerEnd() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayerShutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
}

// Input forwarding
static ImGuiKey CharToImGuiKey(unsigned char c) {
    switch (c) {
        case   8: return ImGuiKey_Backspace;
        case 127: return ImGuiKey_Delete;
        case  13: return ImGuiKey_Enter;
        case  27: return ImGuiKey_Escape;
        case   9: return ImGuiKey_Tab;
        default:  return ImGuiKey_None;
    }
}

void ImGuiLayerKeyDown(unsigned char c) {
    ImGuiIO& io = ImGui::GetIO();
    ImGuiKey k = CharToImGuiKey(c);
    if (k != ImGuiKey_None) io.AddKeyEvent(k, true);
    else                    io.AddInputCharacter(c);
}

void ImGuiLayerKeyUp(unsigned char c) {
    ImGuiKey k = CharToImGuiKey(c);
    if (k != ImGuiKey_None) ImGui::GetIO().AddKeyEvent(k, false);
}

void ImGuiLayerAddMousePos(float x, float y)     { ImGui::GetIO().AddMousePosEvent(x, y); }
void ImGuiLayerAddMouseButton(int btn, bool down){ ImGui::GetIO().AddMouseButtonEvent(btn, down); }
bool ImGuiWantsMouse()    { return ImGui::GetIO().WantCaptureMouse; }
bool ImGuiWantsKeyboard() { return ImGui::GetIO().WantCaptureKeyboard; }

// --- Panel ---

void DrawSceneInspector(Scene* scene) {
    ImGui::SetNextWindowPos(ImVec2(10.0f, 10.0f), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(390.0f, 520.0f), ImGuiCond_Once);

    ImGui::Begin("Scene Inspector");

    ImGui::BeginChild("##tree", ImVec2(150.0f, 0), true);
    for (Object* child : scene->GetChildren())
        DrawObjectTree(child);
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("##props", ImVec2(0, 0), true);
    if (selectedObject)
        DrawPropertiesPanel(selectedObject);
    else
        ImGui::TextDisabled("Select an object");
    ImGui::EndChild();

    ImGui::End();
}
