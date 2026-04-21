#pragma once

#include <chrono>
#include <thread>
#include <cstdio>
#include <cstdlib>

#include "object/scene_object.h"

#define ENG_DEFAULT_FPS 100
#define ENG_DEFAULT_WINDOW_WID 1200
#define ENG_DEFAULT_WINDOW_HEI 700
#define ENG_DEFAULT_WINDOW_POS_X 100
#define ENG_DEFAULT_WINDOW_POS_Y 100

// --- Global State Variables ---
extern int window_wid, window_hei;
extern int mouse_center_x, mouse_center_y;

static float current_fps;
static float current_deltatime;
static bool is_paused;
static double timer;
static double warp_time;

// Use C++ chrono for precise and safe timekeeping
static std::chrono::high_resolution_clock::time_point last_time;
static std::chrono::nanoseconds target_delta_nano;

static Scene* scene;
// DebugText* objDebug;

// --- Forward Declarations ---
void EngineInit(int* argc, char** argv);
void EngineStartLoop();
void SetTargetFPS(unsigned int fps);
float GetCurrentFPS();
double GetTimer();
static void Tick(float deltatime);
Scene* GetScene();
void SetScene(Scene* scene_);

// Callback functions
static void DisplayFunction();
static void IdleFunction();
static void ReshapeFunction(int w, int h);
void PauseGame();