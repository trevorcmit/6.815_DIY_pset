#ifndef A9_H_PHUDVTKB
#define A9_H_PHUDVTKB

#include "Image.h"
#include "basicImageManipulation.h"
#include "filtering.h"
#include <cmath>

// Write your declarations here, or extend the Makefile if you add source files

Image L1_energy(const Image &im);

Image vert_cumulative_energy_map(const Image &im);

Image hori_cumulative_energy_map(const Image &im);

Image seam_carving(const Image &im);

#endif /* end of include guard: A9_H_PHUDVTKB */
