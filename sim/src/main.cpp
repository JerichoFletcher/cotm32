#include <cstdio>
#include "verilated.h"
#include "Vtop_cotm32.h"
#include "Vtop_processor_core.h"

#include "verilated_container.hpp"
#include "load_elf.hpp"
#include "sdl_window.hpp"
#include "imgui_renderer.hpp"
#include "simulator.hpp"

#include "drawers/time_drawer.hpp"
#include "drawers/sidebar.hpp"

int main(int argc, char** argv) {
  // Set up window
  SdlWindow window;
  ImGuiRenderer imgui(window.sdl_win(), window.gl());
  window.add_event_listener(&imgui);
  window.add_render_listener(&imgui);
  
  // Set up simulator and link to renderer
  VerilatedContainer v(argc, argv);
  Simulator sim(v);

  window.add_update_litener(&sim);
  imgui.add_drawer(&sim);

  // Create controllers
  TimeController c_time;

  sim.add_update_listener(&c_time);

  // Create drawers
  TimeDrawer d_time(c_time);
  Sidebar d_sidebar;

  sim.add_render_listener(&d_time);
  sim.add_render_listener(&d_sidebar);

  // Run simulation
  #ifdef BOOT_ROM_PATH
  load_elf(BOOT_ROM_PATH, v);
  #endif
  v.start();
  window.run();
  v.finish();

  return 0;
}
