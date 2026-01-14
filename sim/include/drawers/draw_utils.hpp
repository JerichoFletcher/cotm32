#pragma once

#include <string>

#include "imgui.h"

namespace cotm32::draw_utils {
void draw_signal(
    const char* label, bool on, ImU32 on_color, ImU32 off_color, bool use_avail_region
);
void draw_signal(
    const std::string& label, bool on, ImU32 on_color, ImU32 off_color, bool use_avail_region
);
}  // namespace cotm32::draw_utils
