#pragma once

#include <string>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>

class AppContext {
public:
  SDL_Window* window;
  SDL_GLContext gl_context;
  const char* glsl_version;
  ImGuiIO* io;
  bool done;

  AppContext();
  ~AppContext();
  ImGuiIO& getIO();
  void poll_events();
  void start_frame();
  void end_frame();

  const int FPS = 30;
  const int frame_delay = 1000 / FPS;
  Uint32 frame_start;
  int frame_time;
};

int makeAppContext(AppContext &app);