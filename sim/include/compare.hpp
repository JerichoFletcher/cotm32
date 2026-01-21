#pragma once

#define MAX(a, b) ((a > b) ? (a) : (b))
#define MIN(a, b) ((a < b) ? (a) : (b))

// Subtract v by d as if v is unsigned; clamp the result to 0
#define U_SUBCLAMP0(v, d) ((v > d) ? (v - d) : 0)
