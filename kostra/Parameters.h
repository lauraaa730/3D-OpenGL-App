#pragma once
#include "pgr.h"

constexpr float SCENE_WIDTH = 1.0f;
constexpr float SCENE_HEIGHT = 1.0f;
constexpr float SCENE_DEPTH = 1.0f;

constexpr float FAR_CLIPPING_PLANE = 100.f;

constexpr int WINDOW_WIDTH = 1080;
constexpr int WINDOW_HEIGHT = 850;
constexpr char WINDOW_TITLE[] = "PGR: dudkolau";

enum { KEY_LEFT, KEY_RIGHT, KEY_FORWARD, KEY_BACKWARDS, KEY_UP, KEY_DOWN, KEY_SWITCH_CAMERA, KEY_GET_STATUS, KEYS_COUNT };

bool keyMap[KEYS_COUNT];

