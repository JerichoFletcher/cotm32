#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <imgui.h>

#include "sdl_window.hpp"

struct ImGuiDrawer {
  virtual void draw() = 0;
  virtual ~ImGuiDrawer() = default;
};

class ImGuiRenderer : public SdlWindowEventListener, public SdlWindowRenderListener {
  public:
    ImGuiRenderer(SDL_Window* window, SDL_GLContext gl);
    ~ImGuiRenderer();

    inline ImGuiIO& io() const { return ImGui::GetIO(); }

    void begin_frame();
    void end_frame();
    void add_drawer(ImGuiDrawer* drawer);
    
    void render() override;
    void handle_event(const SDL_Event& e) override;

  private:
    SDL_Window* m_window;
    std::vector<ImGuiDrawer*> m_drawers;
};
