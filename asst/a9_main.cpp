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
  Image pigbird("./Input/pigbird.png"); // Load images
  Image twocats("./Input/twocats.png"); 
  Image pbl1 = vert_cumulative_energy_map(pigbird); // Make CME's
  Image tcl1 = vert_cumulative_energy_map(twocats);

  vector<int> pb_seam = vert_minimum_seam(pbl1); // Get seams
  vector<int> tc_seam = vert_minimum_seam(tcl1);

  for (int n = 0; n < pigbird.height(); n++) {
    int h = pigbird.height() - 1 - n;
    pigbird(pb_seam.at(h), h, 0) = 1.0f;
    pigbird(pb_seam.at(h), h, 1) = 0.0f;
    pigbird(pb_seam.at(h), h, 2) = 0.0f;
  }

  for (int n = 0; n < twocats.height(); n++) {
    int h = twocats.height() - 1 - n;
    twocats(tc_seam.at(h), h, 0) = 1.0f;
    twocats(tc_seam.at(h), h, 1) = 0.0f;
    twocats(tc_seam.at(h), h, 2) = 0.0f;
  }

  pigbird.write("./Output/test_vert_seam_pigbird.png");
  twocats.write("./Output/test_vert_seam_twocats.png");
}


int main() {
  clock_t start = clock();

  test_L1_energy();
  test_vert_cme();
  test_hori_cme();
  test_vert_seam();

  clock_t end = clock();
  double duration = (end - start) * 1.0f / CLOCKS_PER_SEC;
  cout << "a9_main.cpp runtime: " << duration << "s" << endl;
  return 0;
}
