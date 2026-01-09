#include "imgui_layer.hpp"

#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

ImGuiLayer::ImGuiLayer(SDL_Window* window, SDL_GLContext gl):
  m_window(window),
  m_drawers(std::vector<ImGuiDrawer*>()) {
  ImGui::CreateContext();
  auto& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  ImGui::StyleColorsDark();
  ImGui_ImplSDL2_InitForOpenGL(window, gl);
  ImGui_ImplOpenGL3_Init("#version 330");
}

ImGuiLayer::~ImGuiLayer() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}

void ImGuiLayer::begin_frame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();
}

void ImGuiLayer::end_frame() {
  ImGui::Render();
  
  auto& io = this->io();
  glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::frame_callback() {
  this->begin_frame();
  for (auto* p : this->m_drawers) {
    p->draw();
  }
  this->end_frame();
}

void ImGuiLayer::add_drawer(ImGuiDrawer* drawer) {
  this->m_drawers.push_back(drawer);
}

void ImGuiLayer::handle_event(const SDL_Event& e) {
  ImGui_ImplSDL2_ProcessEvent(&e);
}
