cmake_minimum_required(VERSION 3.16)
project(cotm32 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Dependencies
find_package(SDL2 REQUIRED)
find_package(VERILATOR REQUIRED)
find_package(fmt CONFIG REQUIRED)

find_program(RISCV_GCC      riscv64-unknown-elf-gcc REQUIRED)

# Bootloader
set(BOOT_SRC
  ${CMAKE_SOURCE_DIR}/sw/boot/boot.s
  ${CMAKE_SOURCE_DIR}/sw/boot/func.s
  ${CMAKE_SOURCE_DIR}/sw/boot/trap.s
  ${CMAKE_SOURCE_DIR}/sw/boot/sys.s
  ${CMAKE_SOURCE_DIR}/sw/boot/exc.s
  ${CMAKE_SOURCE_DIR}/sw/boot/interr.s
)
set(BOOT_INC  ${CMAKE_SOURCE_DIR}/sw/include)
set(BOOT_LD   ${CMAKE_SOURCE_DIR}/sw/boot/boot.ld)
set(BOOT_ELF  ${CMAKE_BINARY_DIR}/boot.elf)

add_custom_command(
  OUTPUT ${BOOT_ELF}
  COMMAND ${RISCV_GCC}
    -march=rv32im_zicsr
    -mabi=ilp32
    -nostdlib
    -I ${BOOT_INC}
    -T ${BOOT_LD}
    -o ${BOOT_ELF}
    ${BOOT_SRC}
  DEPENDS
    ${BOOT_SRC}
    ${BOOT_LD}
  COMMENT "Building RISC-V bootloader"
)

add_custom_target(bootloader ALL
  DEPENDS ${BOOT_ELF}
)

# Verilator model
set(CXX_SOURCES
  sim/src/main.cpp
  sim/src/verilated_container.cpp
  sim/src/load_elf.cpp
  sim/src/sdl_window.cpp
  sim/src/imgui_renderer.cpp
  sim/src/simulator.cpp
  sim/src/name_utils.cpp

  sim/src/views/reg_view.cpp
  sim/src/views/csr_view.cpp
  sim/src/views/mem_view.cpp
  sim/src/views/trap_view.cpp
  sim/src/views/pipeline_regs_view.cpp
  sim/src/views/fwd_view.cpp
  sim/src/views/clint_view.cpp

  sim/src/drawers/draw_utils.cpp
  sim/src/drawers/sidebar.cpp
  sim/src/drawers/time_drawer.cpp
  sim/src/drawers/dump_drawer.cpp
  sim/src/drawers/term_drawer.cpp
  sim/src/drawers/reg_drawer.cpp
  sim/src/drawers/csr_drawer.cpp
  sim/src/drawers/mem_drawer.cpp
  sim/src/drawers/trap_drawer.cpp
  sim/src/drawers/pipeline_regs_drawer.cpp
  sim/src/drawers/fwd_drawer.cpp
  sim/src/drawers/clint_drawer.cpp

  sim/src/controllers/time_controller.cpp
  sim/src/controllers/dump_controller.cpp
  sim/src/controllers/term_controller.cpp
)

set(RTL_SOURCES
  rtl/cotm32_pkg.sv
  rtl/cotm32_priv_pkg.sv
  rtl/cotm32_pipeline_pkg.sv
  rtl/cotm32.sv

  rtl/comp/dec.sv
  rtl/comp/mux.sv

  rtl/cpu/processor_core.sv

  rtl/cpu/core/register_file.sv
  rtl/cpu/core/alu.sv
  rtl/cpu/core/bu.sv
  rtl/cpu/core/cu.sv
  rtl/cpu/core/data_mem.sv
  rtl/cpu/core/inst_fetch.sv
  rtl/cpu/core/lsu.sv
  rtl/cpu/core/rom.sv
  rtl/cpu/core/sign_ext.sv
  
  rtl/cpu/core/mu.sv

  rtl/cpu/priv/csr_file.sv
  rtl/cpu/priv/exception_dispatch.sv
  rtl/cpu/priv/interrupt_dispatch.sv
  rtl/cpu/priv/trap_control.sv

  rtl/cpu/pipeline/ifid_reg.sv
  rtl/cpu/pipeline/idex_reg.sv
  rtl/cpu/pipeline/exmem_reg.sv
  rtl/cpu/pipeline/memwb_reg.sv
  rtl/cpu/pipeline/fwd_unit.sv
  rtl/cpu/pipeline/hazard_unit.sv

  rtl/peripheral/clint.sv
  rtl/peripheral/uart/uart_term.sv
)

add_executable(cotm32
  ${CXX_SOURCES}

  sim/third_party/imgui/imgui.cpp
  sim/third_party/imgui/imgui_draw.cpp
  sim/third_party/imgui/imgui_tables.cpp
  sim/third_party/imgui/imgui_widgets.cpp
  
  sim/third_party/imgui/backends/imgui_impl_sdl2.cpp
  sim/third_party/imgui/backends/imgui_impl_opengl3.cpp

  sim/third_party/imgui/misc/cpp/imgui_stdlib.cpp
)

verilate(cotm32
  SOURCES ${RTL_SOURCES}    
  TOP_MODULE cotm32
  PREFIX Vtop
  VERILATOR_ARGS
    "-Wall"
    "-Wno-UNUSEDPARAM"
    "-Wno-UNUSEDSIGNAL"
  TRACE
)

add_dependencies(cotm32 bootloader)

# Target definitions
target_sources(cotm32 PRIVATE
  ${CXX_SOURCES}
  ${RTL_SOURCES}
)

target_compile_definitions(cotm32 PRIVATE
  BOOT_ROM_PATH="${BOOT_ELF}"
)

target_include_directories(cotm32 PRIVATE
  sim/include
  sim/third_party/imgui
  sim/third_party/imgui/backends
  sim/third_party/imgui/misc/cpp
  ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(cotm32 PRIVATE
  fmt::fmt
  ${SDL2_LIBRARIES}
  GL
)
