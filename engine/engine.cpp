#include "engine.h"

#include <glad/glad.h>
#include <GL/glut.h>

#include <iostream>

#include "rendering/shader_loader.h"
#include "input/input_mapper.h"

int window_wid, window_hei;
int mouse_center_x, mouse_center_y;

void EngineInit(int* argc, char** argv) {
    // Initialize time variables
    target_delta_nano = std::chrono::nanoseconds((long long)(1e9 / ENG_DEFAULT_FPS));
    current_fps = 0.0f;
    timer = 0.0;
    is_paused = false;
    allowMouseMotion = true;

    window_wid = ENG_DEFAULT_WINDOW_WID;
    window_hei = ENG_DEFAULT_WINDOW_HEI;
    mouse_center_x = window_wid / 2;
    mouse_center_y = window_hei / 2;

    // Initialize objects
    scene = new Scene();

    // Initialize GLUT
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE | GLUT_DEPTH);
    glutInitWindowPosition(ENG_DEFAULT_WINDOW_POS_X, ENG_DEFAULT_WINDOW_POS_Y);
    glutInitWindowSize(window_wid, window_hei);
    glutCreateWindow("Robo Beach");

    if (!gladLoadGL()) {
        std::cerr << "GLAD failed\n";
        return;
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GLUT_MULTISAMPLE);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    InputMapperInit();

    // Register callbacks
    glutDisplayFunc(DisplayFunction);
    glutIdleFunc(IdleFunction);
    glutReshapeFunc(ReshapeFunction);
    // glutSetCursor(GLUT_CURSOR_NONE);
}

void EngineStartLoop() {
    // Record the starting time point
    last_time = std::chrono::high_resolution_clock::now();
    glutMainLoop();
    
}

void SetTargetFPS(unsigned int fps) {
    if (fps == 0 || fps > 10000) return;
    target_delta_nano = std::chrono::nanoseconds((long long)(1e9 / fps));
}

float GetCurrentFPS() {
    return current_fps;
}

double GetTimer() {
    return timer;
}

void Tick(float deltatime) {
    timer += deltatime;

    InputMapperUpdate();
    
    char tmp[256];
    snprintf(tmp, sizeof(tmp),
        "frame rate: %3.1f, deltatime: %7.4f, timer: %7.2f\n",
        GetCurrentFPS(), deltatime, GetTimer()
    );

    if (!is_paused) {
        scene->Update(deltatime);
        scene->CollisionCheck();
    }

    glutPostRedisplay();
}

Scene *GetScene() {
    return scene;
}

void DisplayFunction() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    scene->Render();
    
    glutSwapBuffers();
}

void IdleFunction() {
    auto current_time = std::chrono::high_resolution_clock::now();
    auto elapsed = current_time - last_time;

    // PERFORMANCE FIX: Sleep the thread if we rendered too fast instead of busy-waiting
    if (elapsed < target_delta_nano) {
        std::this_thread::sleep_for(target_delta_nano - elapsed);
        
        // Update current time after waking up
        current_time = std::chrono::high_resolution_clock::now();
        elapsed = current_time - last_time;
    }

    // Calculate actual delta time in seconds
    current_deltatime = std::chrono::duration<float>(elapsed).count();
    
    // Prevent division by zero if delta time is unexpectedly extremely small
    if (current_deltatime > 0.0f) {
        current_fps = 1.0f / current_deltatime;
    }

    last_time = current_time;

    // Execute game logic
    Tick(current_deltatime);
}

void ReshapeFunction(int w, int h) {
    // Prevent division by zero if the window is minimized
    if (h == 0) h = 1;

    window_wid = w;
    window_hei = h;
    mouse_center_x = window_wid / 2;
    mouse_center_y = window_hei / 2;

    double aspectRatio = (double)w / (double)h;

    glViewport(0, 0, w, h);

    if (scene && scene->GetCamera()) {
        scene->GetCamera()->SetAspectRatio(aspectRatio);
    }
}

void PauseGame() {
    is_paused = !is_paused;
    allowMouseMotion = false;

    if (is_paused) {
        glutSetCursor(GLUT_CURSOR_NONE);
        glutWarpPointer(mouse_center_x, mouse_center_y);
        warp_time = timer;
    } else {
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    }
}
