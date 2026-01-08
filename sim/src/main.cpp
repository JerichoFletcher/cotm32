#include <cstdio>
#include "verilated.h"
#include "Vtop_cotm32.h"
#include "Vtop_processor_core.h"

#include "verilated_container.hpp"
#include "load_elf.hpp"
#include "sdl_window.hpp"
#include "imgui_layer.hpp"

struct HelloDrawer : public ImGuiDrawer {
  void draw() {
    ImGui::Begin("My first win!");
    ImGui::Text("Hello!");
    ImGui::End();
  }
};

int main(int argc, char** argv) {
//   VerilatedContainer v(argc, argv);
// #ifdef BOOT_ROM_PATH
//   load_elf(BOOT_ROM_PATH, v);
// #endif

  SdlWindow window;
  ImGuiLayer imgui(window.sdl_win(), window.gl());
  window.add_listener(&imgui);
  window.add_frame_callback(&imgui);

  auto d = HelloDrawer();
  imgui.add_drawer(&d);

  window.run();
}
