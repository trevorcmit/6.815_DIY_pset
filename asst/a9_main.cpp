#include <iostream>
#include "a9.h"
#include "Image.h"
#include <iomanip>
#include <iostream>
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


void test_vert_cme() {
  Image pigbird("./Input/pigbird.png");
  Image twocats("./Input/twocats.png");
  Image pbl1 = vert_cumulative_energy_map(pigbird);
  Image tcl1 = vert_cumulative_energy_map(twocats);
  pbl1.write("./Output/test_vert_cme_pigbird.png");
  tcl1.write("./Output/test_vert_cme_twocats.png");
};


int main() {
  clock_t start = clock();

  test_L1_energy();
  test_vert_cme();

  clock_t end = clock();
  double duration = (end - start) * 1.0f / CLOCKS_PER_SEC;
  cout << "a9_main.cpp runtime: " << duration << "s" << endl;
  return 0;
}
