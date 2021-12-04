/* --------------------------------------------------------------------------
 * File:    a9.cpp
 * Created: 2021-12-2
 * --------------------------------------------------------------------------
 *
 *
 *
 * ------------------------------------------------------------------------*/

#include "a9.h"
#include "filtering.h"
#include "Image.h"
#include "basicImageManipulation.h"

using namespace std;


Image L1_energy(const Image &im) {
    // Compute L1 energy based on gradients based on E(I) = |I_x| + |I_y|
    // Input: A normal three-channel color image.
    // Output: One channel output of L1, E(I), calculated for each pixel.
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
    // Output: One channel image representing vertical CME.
    Image l1 = L1_energy(im);
    Image cem(im.width(), im.height(), 1);      // Initialize output image

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
    // Calculate cumulative energy map in the horizontal direction.
    // Input: A normal color image. Function computes L1 itself.
    // Output: One channel image representing horizontal CME.
    Image l1 = L1_energy(im);
    Image cem(im.width(), im.height(), 1);       // Initialize output image

    for (int y = 0; y < im.height(); y++) {      // Copy left row of input energy map
        cem(0, y) = l1(0, y);
    }

    for (int w = 1; w < im.width(); w++) {      // Iterate over column [1,...,n]
        for (int h = 0; h < im.height(); h++) {  
            if (h == 0) {
                cem(w, h) = l1(w, h) + std::min(cem(w-1, h), cem(w-1, h+1)); // Top edge case
            }
            else if (h == im.height() - 1) {
                cem(w, h) = l1(w, h) + std::min(cem(w-1, h), cem(w-1, h-1)); // Bottom edge case
            }
            else {
                cem(w, h) = l1(w, h) + std::min(std::min(cem(w-1, h), cem(w-1, h-1)), cem(w-1, h+1));
            }
        }
    }
    cem = cem / cem.max(); // Scale from [0,1]
    return cem;            // Return cumulative energy map
}


vector<int> vert_minimum_seam(const Image &cme) {
    // Calculate the minimum seam of an input vertical cumulative energy map.
    // Input: One channel image representing vertical CME.
    // Output: Vector of integer indices representing row indices of minimum energy.
    vector<int> output; // Initialize output indices vector
    float min = 1000.0f;     // Initialize value for mininum energy
    int index = 1000;   // Initailize variable for min. energy index

    for (int x = 0; x < cme.width(); x++) {
        if (cme(x, cme.height()-1) < min) {
            min = cme(x, cme.height()-1);
            index = x;
        }
    }
    output.push_back(index);

    for (int y = 0; y <= cme.height() - 2; y++) {
        int h = cme.height() - 2 - y;

        if (index == 0) { // Left edge case, two options for next index
            float m = cme(0, h);
            float r = cme(1, h);
            if (m < r) {index = 0;}
            else       {index = 1;}
        }

        else if (index == cme.width()-1) { // Right edge case, two options for next index
            float m = cme(cme.width()-1, h);
            float l = cme(cme.width()-2, h);
            if (m < l) {index = cme.width()-1;}
            else       {index = cme.width()-2;}
        }

        else { // Middle case, three options for next index
            float l = cme(index-1, h);
            float m = cme(index, h);
            float r = cme(index+1, h);
            if ((l < m) && (l < r)) {index = index - 1;}      // Left is new min index
            else if ((r < m) && (r < m)) {index = index + 1;} // Right is new min index
        }

        output.push_back(index);
    }

    return output;
}


vector<int> hori_minimum_seam(const Image &cme) {
    // Calculate the minimum seam of an input horizontal cumulative energy map.
    // Input: One channel image representing horizontal CME.
    // Output: Vector of integer indices representing column indices of minimum energy.
    vector<int> output;
    return output;
}


Image seam_carving(const Image &im) {
    Image output(im.width(), im.height(), im.channels());
    return output;
}
