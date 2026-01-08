#pragma once

#include <memory>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

struct SdlWindowEventListener {
  virtual void handle(const SDL_Event& e) = 0;
  virtual ~SdlWindowEventListener() = default;
};

struct SdlWindowFrameCallback {
  virtual void frame_callback() = 0;
  virtual ~SdlWindowFrameCallback() = default;
};

class SdlWindow {
  public:
    SdlWindow();
    ~SdlWindow();

    inline SDL_Window* sdl_win() const { return this->m_window; }
    inline SDL_GLContext gl() const { return this->m_gl; }

    void add_listener(SdlWindowEventListener* listener);
    void add_frame_callback(SdlWindowFrameCallback* fc);
    void run();

  private:
    SDL_Window* m_window;
    SDL_GLContext m_gl;
    std::vector<SdlWindowEventListener*> m_listeners;
    std::vector<SdlWindowFrameCallback*> m_fc;
};
