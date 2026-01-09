#include "sdl_window.hpp"

SdlWindow::SdlWindow():
  m_listeners(std::vector<SdlWindowEventListener*>()),
  m_fc(std::vector<SdlWindowFrameCallback*>()) {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  this->m_window = SDL_CreateWindow(
    "COTM32",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    1280, 720,
    SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED
  );
  this->m_gl = SDL_GL_CreateContext(this->m_window);
  SDL_GL_MakeCurrent(this->m_window, this->m_gl);
  SDL_GL_SetSwapInterval(1);
}

SdlWindow::~SdlWindow() {
  SDL_DestroyWindow(this->m_window);
  SDL_Quit();
}

void SdlWindow::add_listener(SdlWindowEventListener* listener) {
  this->m_listeners.push_back(listener);
}

void SdlWindow::add_frame_callback(SdlWindowFrameCallback* fc) {
  this->m_fc.push_back(fc);
}

void SdlWindow::run() {
  bool running = true;
  while (running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      for (auto* l : this->m_listeners) {
        l->handle_event(e);
      }

      if (e.type == SDL_QUIT) {
        running = false;
      }
    }

    for (auto* fc : this->m_fc) {
      fc->frame_callback();
    }
    SDL_GL_SwapWindow(this->m_window);
  }
}
