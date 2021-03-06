/* --------------------------------------------------------------------------
 * File:    a9_main.cpp
 * Created: 2021-12-2
 * --------------------------------------------------------------------------
 *
 *
 *
 * ------------------------------------------------------------------------*/

#include <iostream>
#include "a9.h"
#include "Image.h"
#include "filtering.h"
#include <iomanip>
#include <sstream>
#include <vector>

using namespace std;


void test_L1_energy() { // Test L1 energy helper function. Returns two L1 energy calculations
  Image pigbird("./Input/pigbird.png");
  Image twocats("./Input/twocats.png");
  Image pbl1 = L1_energy(pigbird);
  Image tcl1 = L1_energy(twocats);
  pbl1.write("./Output/test_L1_pigbird.png");
  tcl1.write("./Output/test_L1_twocats.png");
};


void test_vert_cme() { // Test vert_cumulative_energy_map function on two images.
  Image pigbird("./Input/pigbird.png");
  Image twocats("./Input/twocats.png");
  Image pbl1 = vert_cumulative_energy_map(pigbird);
  Image tcl1 = vert_cumulative_energy_map(twocats);
  pbl1.write("./Output/test_vert_cme_pigbird.png");
  tcl1.write("./Output/test_vert_cme_twocats.png");
};


void test_hori_cme() { // Test vert_cumulative_energy_map function on two images.
  Image pigbird("./Input/pigbird.png");
  Image twocats("./Input/twocats.png");
  Image pbl1 = hori_cumulative_energy_map(pigbird);
  Image tcl1 = hori_cumulative_energy_map(twocats);
  pbl1.write("./Output/test_hori_cme_pigbird.png");
  tcl1.write("./Output/test_hori_cme_twocats.png");
};


void test_vert_seam() {
  Image pigbird("./Input/pigbird.png");              // Load images
  Image twocats("./Input/twocats.png"); 
  Image pbl1 = vert_cumulative_energy_map(pigbird);  // Make CME's
  Image tcl1 = vert_cumulative_energy_map(twocats);
  vector<int> pb_seam = vert_minimum_seam(pbl1);     // Get seams
  vector<int> tc_seam = vert_minimum_seam(tcl1);

  for (int n = 0; n < pigbird.height(); n++) { // Highlight seam on Pigbird
    int h = pigbird.height() - 1 - n;
    pigbird(pb_seam.at(h), h, 0) = 1.0f;
    pigbird(pb_seam.at(h), h, 1) = 0.0f;
    pigbird(pb_seam.at(h), h, 2) = 0.0f;
  }
  for (int n = 0; n < twocats.height(); n++) { // Highlight seam on Twocats
    int h = twocats.height() - 1 - n;
    twocats(tc_seam.at(h), h, 0) = 1.0f;
    twocats(tc_seam.at(h), h, 1) = 0.0f;
    twocats(tc_seam.at(h), h, 2) = 0.0f;
  }
  pigbird.write("./Output/test_vert_seam_pigbird.png");
  twocats.write("./Output/test_vert_seam_twocats.png");
}


void test_hori_seam() {
  Image pigbird("./Input/pigbird.png");              // Load images
  Image twocats("./Input/twocats.png"); 
  Image pbl1 = hori_cumulative_energy_map(pigbird);  // Make CME's
  Image tcl1 = hori_cumulative_energy_map(twocats);
  vector<int> pb_seam = hori_minimum_seam(pbl1);     // Get seams
  vector<int> tc_seam = hori_minimum_seam(tcl1);

  for (int n = 0; n < pigbird.width(); n++) { // Highlight seam on Pigbird
    int w = pigbird.width() - 1 - n;
    pigbird(w, pb_seam.at(w), 0) = 1.0f;
    pigbird(w, pb_seam.at(w), 1) = 0.0f;
    pigbird(w, pb_seam.at(w), 2) = 0.0f;
  }
  for (int n = 0; n < twocats.width(); n++) { // Highlight seam on Twocats
    int w = twocats.width() - 1 - n;
    twocats(w, tc_seam.at(w), 0) = 1.0f;
    twocats(w, tc_seam.at(w), 1) = 0.0f;
    twocats(w, tc_seam.at(w), 2) = 0.0f;
  }
  pigbird.write("./Output/test_hori_seam_pigbird.png");
  twocats.write("./Output/test_hori_seam_twocats.png");
}


void test_vertical_seam_carving() {
  Image pigbird("./Input/pigbird.png");                                 // Load images
  Image twocats("./Input/twocats.png"); 
  Image pb_carved = vertical_seam_carving(pigbird, pigbird.width()/3);  // Seam carve 1/3 of width
  Image tc_carved = vertical_seam_carving(twocats, twocats.width()/3);
  pb_carved.write("./Output/test_vertical_carving_pigbird.png");   // Write to output
  tc_carved.write("./Output/test_vertical_carving_twocats.png");
}


void test_horizontal_seam_carving() {
  Image pigbird("./Input/pigbird.png");                                   // Load images
  Image twocats("./Input/twocats.png"); 
  Image pb_carved = horizontal_seam_carving(pigbird, pigbird.height()/3);  // Seam carve 1/3 of height
  Image tc_carved = horizontal_seam_carving(twocats, twocats.height()/3);
  pb_carved.write("./Output/test_horizontal_carving_pigbird.png");   // Write to output
  tc_carved.write("./Output/test_horizontal_carving_twocats.png");
}


void test_object_removal_seam_carving() {
  Image pigbird("./Input/pigbird.png");                                    // Load images
  Image twocats("./Input/twocats.png"); 
  vector<int> tc_vector = {184, 260, 40, 112};                             // Regions for removal
  vector<int> pb_vector = {92, 178, 37, 155};
  Image pb_carved = object_removal_seam_carving(pigbird, pb_vector, true); // Carve object
  Image tc_carved = object_removal_seam_carving(twocats, tc_vector, true);
  pb_carved.write("./Output/test_object_removal_pigbird.png");             // Write to output
  tc_carved.write("./Output/test_object_removal_twocats.png");
}


void test_object_removal_extra() {
  Image soccer("./Input/soccer.png");   // Load images
  Image skydive("./Input/skydive.png");
  vector<int> soccer_vector = {39, 125, 13, 308};   // Define regions
  vector<int> skydive_vector = {60, 161, 162, 238};
  Image soccer_carved = object_removal_seam_carving(soccer, soccer_vector, true);     // vertical carve
  Image skydive_carved = object_removal_seam_carving(skydive, skydive_vector, false); // horizontal carve
  soccer_carved.write("./Output/test_object_removal_soccer.png");
  skydive_carved.write("./Output/test_object_removal_skydive.png"); // Output into folder
}



int main() {
  clock_t start = clock();

  test_L1_energy();
  test_vert_cme();
  test_hori_cme();
  test_vert_seam();
  test_hori_seam();
  test_vertical_seam_carving();
  test_horizontal_seam_carving();
  test_object_removal_seam_carving();
  test_object_removal_extra();

  clock_t end = clock();
  double duration = (end - start) * 1.0f / CLOCKS_PER_SEC;
  cout << "a9_main.cpp runtime: " << duration << "s" << endl;
  return 0;
}
