#include <cstdio>
#include "verilated.h"
#include "Vtop_cotm32.h"
#include "Vtop_processor_core.h"

#include "verilated_container.hpp"
#include "load_elf.hpp"
#include "sdl_window.hpp"
#include "imgui_layer.hpp"

#include "drawers/time_drawer.hpp"
#include "drawers/reg_drawer.hpp"
#include "drawers/csr_drawer.hpp"

int main(int argc, char** argv) {
  VerilatedContainer v(argc, argv);
#ifdef BOOT_ROM_PATH
  load_elf(BOOT_ROM_PATH, v);
#endif
  v.start();

  SdlWindow window;
  ImGuiLayer imgui(window.sdl_win(), window.gl());
  window.add_listener(&imgui);
  window.add_frame_callback(&imgui);

  auto d_time = TimeDrawer(v);
  imgui.add_drawer(&d_time);
  auto d_reg = RegDrawer(v);
  imgui.add_drawer(&d_reg);
  auto d_csr = CsrDrawer(v);
  imgui.add_drawer(&d_csr);

  window.run();
  v.finish();

  return 0;
}
