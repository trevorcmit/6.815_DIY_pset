// #include <iostream>
#include "a9.h"
#include "filtering.h"
#include "Image.h"
#include "basicImageManipulation.h"

using namespace std;


Image L1_energy(const Image &im) {
    // Compute L1 energy based on gradients based on E(I) = |I_x| + |I_y|
    // Input: A normal color image.
    Image gray = color2gray(im);
    Image grad_x = gradientX(gray);        // Get X gradient
    Image grad_y = gradientY(gray);        // Get Y gradient

    Image output(im.width(), im.height(), 1);
    for (int h = 0; h < im.height(); h++) {                       // Iterate over rows [1,...,n]
        for (int w = 0; w < im.width(); w++) {
            output(w, h) = abs(grad_x(w, h)) + abs(grad_y(w, h)); // Add absolute values together
        }
    }
    output = output / output.max();        // Normalize to [0,1]
    return output;                         // Return E(I) map
}


Image vert_cumulative_energy_map(const Image &im) {
    // Calculate cumulative energy map in the vertical direction.
    // Input: A normal color image. Function computes L1 itself.
    Image l1 = L1_energy(im);
    Image cem(im.width(), im.height(), 1); // Initialize output image

    for (int x = 0; x < im.width(); x++) {      // Copy top row of input energy map
        cem(x, 0) = l1(x, 0);
    }

    for (int h = 1; h < im.height(); h++) {     // Iterate over rows [1,...,n]
        for (int w = 0; w < im.width(); w++) {
            if (w == 0) {
                cem(w, h) = l1(w, h) + std::min(cem(w, h-1), cem(w+1, h-1)); // Left edge case
            }
            else if (w == im.width() - 1) {
                cem(w, h) = l1(w, h) + std::min(cem(w, h-1), cem(w-1, h-1)); // Right edge case
            }
            else {
                cem(w, h) = l1(w, h) + std::min(std::min(cem(w, h-1), cem(w-1, h-1)), cem(w+1, h-1));
            }
        }
    }
    cem = cem / cem.max(); // Scale from [0,1]
    return cem;            // Return cumulative energy map
}


Image hori_cumulative_energy_map(const Image &im) {
    Image output(im.width(), im.height(), im.channels());
    return output;
}


Image seam_carving(const Image &im) {
    Image output(im.width(), im.height(), im.channels());
    return output;
}
