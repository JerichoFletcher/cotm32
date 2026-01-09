#include "controllers/dump_controller.hpp"
#include "controllers/time_controller.hpp"
#include "drawers/dump_drawer.hpp"
#include "drawers/sidebar.hpp"
#include "drawers/time_drawer.hpp"
#include "imgui_renderer.hpp"
#include "load_elf.hpp"
#include "sdl_window.hpp"
#include "simulator.hpp"
#include "verilated_container.hpp"

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
    DumpController c_dump(c_time);
    sim.add_update_listener(&c_time);
    sim.add_update_listener(&c_dump);

    // Create drawers
    Sidebar d_sidebar;
    TimeDrawer d_time(c_time);
    DumpDrawer d_dump(c_dump);
    sim.add_render_listener(&d_sidebar);
    sim.add_render_listener(&d_time);
    sim.add_render_listener(&d_dump);

    // Run simulation
#ifdef BOOT_ROM_PATH
    load_elf(BOOT_ROM_PATH, v);
#endif
    v.start();
    window.run();
    v.finish();

    return 0;
}
