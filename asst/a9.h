/* --------------------------------------------------------------------------
 * File:    a9.h
 * Created: 2021-12-2
 * --------------------------------------------------------------------------
 *
 *
 *
 * ------------------------------------------------------------------------*/

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

vector<int> vert_minimum_seam(const Image &cme);

vector<int> hori_minimum_seam(const Image &cme);

Image vertical_seam_carving(const Image &im, int N);

Image horizontal_seam_carving(const Image &im, int N);

Image optimal_order_seam_carving(const Image &im, int new_width, int new_height);

Image object_removal_seam_carving(const Image &im, vector<int> remove_region, bool vertical);

#endif /* end of include guard: A9_H_PHUDVTKB */
