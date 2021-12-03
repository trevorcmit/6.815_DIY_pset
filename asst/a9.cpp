#include <iostream>
#include "a9.h"
#include "filtering.h"
#include "basicImageManipulation.h"

using namespace std;

Image gradientX(const Image &im, bool clamp) { // Helper function taken from PS07
    // Sobel filtering in X direction
  Filter sobelX(3, 3);
  sobelX(0, 0) = -1.0;
  sobelX(1, 0) = 0.0;
  sobelX(2, 0) = 1.0;
  sobelX(0, 1) = -2.0;
  sobelX(1, 1) = 0.0;
  sobelX(2, 1) = 2.0;
  sobelX(0, 2) = -1.0;
  sobelX(1, 2) = 0.0;
  sobelX(2, 2) = 1.0;
  Image imSobelX = sobelX.convolve(im, clamp);
  return imSobelX;
}


Image gradientY(const Image &im, bool clamp) { // Helper function taken from PS07
  // Sobel filtering in Y direction
  Filter sobelY(3, 3);
  sobelY(0, 0) = -1.0;
  sobelY(1, 0) = -2.0;
  sobelY(2, 0) = -1.0;
  sobelY(0, 1) = 0.0;
  sobelY(1, 1) = 0.0;
  sobelY(2, 1) = 0.0;
  sobelY(0, 2) = 1.0;
  sobelY(1, 2) = 2.0;
  sobelY(2, 2) = 1.0;
  Image imSobelY = sobelY.convolve(im, clamp);
  return imSobelY;
}


Image L1_energy(const Image &im) {
    // Compute L1 energy based on gradients based on E(I) = |I_x| + |I_y|
    // Input: A normal color image.
    Image grad_x = gradientX(im);          // Get X gradient
    Image grad_y = gradientY(im);          // Get Y gradient
    Image output = grad_x + grad_y;        // Add together
    Image output = output / output.max();  // Normalize to [0,1]
    return output;                         // Return E(I) map
}


Image vert_cumulative_energy_map(const Image &im) {
    // Calculate cumulative energy map in the vertical direction.
    // Input: A normal color image. Function computes L1 itself.
    Image l1 = L1_energy(im);
    Image cem(im.width(), im.height(), im.channels()); // Initialize output image

    for (int x = 0; x < im.width(); x++) {                // Copy top row of input energy map
        cem(x, 0) = l1(x, 0);
    }

    for (int h = 1; h < im.height(); h++) {     // Iterate over rows [1,...,n]
        for (int w = 0; w < im.width(); w++) {
            if (w == 0) {
                cem(w, h) = l1(w, h) + min(cem(w, h-1), cem(w+1, h-1)); // Left edge case
            }
            else if (w == im.width() - 1) {
                cem(w, h) = l1(w, h) + min(cem(w, h-1), cem(w-1, h-1)); // Right edge case
            }
            else {
                cem(w, h) = l1(w, h) + min(cem(w, h-1), cem(w-1, h-1), cem(w+1, h-1));
            }
        }
    }
    return cem; // Return cumulative energy map
}

Image hori_cumulative_energy_map() {

}


Image seam_carving(const Image &im) {
    Image gray = color2gray(im);    // Get grayscale version of image
}
