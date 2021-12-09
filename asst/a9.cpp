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
    vector<int> output;      // Initialize output indices vector
    float min = 1000.0f;     // Initialize value for mininum energy
    int index = 1000;        // Initailize variable for min. energy index

    for (int x = 0; x < cme.width(); x++) { // Iterate over bottom row to get starting index
        if (cme(x, cme.height()-1) < min) {
            min = cme(x, cme.height()-1);   // Store minimum for next iteration
            index = x;                      // Set new starting index if energy less than existing
        }
    }
    output.push_back(index);

    for (int y = 0; y <= cme.height() - 2; y++) { // Iterate over all rows except bottom row
        int h = cme.height() - 2 - y;

        if (index == 0) { // Left edge case, two options for next index
            if (cme(0, h) < cme(1, h)) {index = 0;}
            else                       {index = 1;}
        }
        else if (index == cme.width()-1) { // Right edge case, two options for next index
            if (cme(cme.width()-1, h) < cme(cme.width()-2, h)) {index = cme.width()-1;}
            else                                               {index = cme.width()-2;}
        }
        else { // Middle case, three options for next index
            float l = cme(index-1, h);
            float m = cme(index, h);
            float r = cme(index+1, h);
            if ((l < m) && (l < r))      {index = index - 1;} // Left is new min index
            else if ((r < m) && (r < l)) {index = index + 1;} // Right is new min index
        }
        output.push_back(index);
    }
    return output;
}


vector<int> hori_minimum_seam(const Image &cem) {
    // Calculate the minimum seam of an input horizontal cumulative energy map.
    // Input: One channel image representing horizontal CME.
    // Output: Vector of integer indices representing column indices of minimum energy.
    vector<int> output;      // Initialize output indices vector
    float min = 1000.0f;     // Initialize value for mininum energy
    int index = 1000;        // Initailize variable for min. energy index

    for (int y = 0; y < cem.height(); y++) { // Iterate over right column to get starting index
        if (cem(cem.width()-1, y) < min) {
            min = cem(cem.width()-1, y);     // Store minimum for next iteration
            index = y;                       // Set new starting index if energy less than existing
        }
    }
    output.push_back(index);

    for (int x = 0; x <= cem.width() - 2; x++) {     // Iterate over all rows except bottom row
        int w = cem.width() - 2 - x;

        if (index == 0) {                            // Top edge case, two options for next index
            if (cem(w, 0) < cem(w, 1)) {index = 0;}
            else                       {index = 1;}
        }
        else if (index == cem.height()-1) {          // Bottom edge case, two options for next index
            if (cem(w, cem.height()-1) < cem(w, cem.height()-2)) {index = cem.height()-1;}
            else                                                 {index = cem.height()-2;}
        }
        else { // Middle case, three options for next index
            float a = cem(w, index-1); // New index is above
            float m = cem(w, index);   // New index is the same
            float b = cem(w, index+1); // new index is below
            if ((a < m) && (a < b))      {index = index - 1;} // Left is new min index
            else if ((b < m) && (b < a)) {index = index + 1;} // Right is new min index
        }
        output.push_back(index);
    }
    return output;
}


Image vertical_seam_carving(const Image &im, int N) {
    // Seam carve an input image N number of times, removing vertical seams.
    // Input: Three-channel input image.
    // Output: Input image seam carved N number of times (width reduced by N).
    vector<Image> carving; // Initialize vector to store in-progress carved images.
    carving.push_back(im);

    for (int n = 0; n < N; n++) {
        Image vcme = vert_cumulative_energy_map(carving.at(n));    // Get cumulative energy map
        vector<int> seam = vert_minimum_seam(vcme);                // Find current min. seam
        Image current(im.width()-n-1, im.height(), im.channels()); // Initialize next carving

        for (int h = 0; h < im.height(); h++) {        // Iterate over all rows
            int i = 0;
            for (int w = 0; w < im.width()-n; w++) { // Iterate over current width
                if (w != seam.at(current.height()-h-1)) {  // Check that current index is not seam index
                    current(i, h, 0) = carving.at(n)(w, h, 0);
                    current(i, h, 1) = carving.at(n)(w, h, 1);
                    current(i, h, 2) = carving.at(n)(w, h, 2);
                    i += 1;
                }
            }
        }
        carving.push_back(current); // Add in-progress carved image to vector to call next iteration.
    }
    return carving.at(N); // Return final carved image, carved N number of times.
}


Image horizontal_seam_carving(const Image &im, int N) {
    // Seam carve an input image N number of times, removing horizontal seams.
    // Input: Three-channel input image.
    // Output: Input image seam carved N number of times (height reduced by N).
    vector<Image> carving; // Initialize vector to store in-progress carved images.
    carving.push_back(im);

    for (int n = 0; n < N; n++) {
        Image hcme = hori_cumulative_energy_map(carving.at(n));    // Get cumulative energy map
        vector<int> seam = hori_minimum_seam(hcme);                // Find current min. seam
        Image current(im.width(), im.height()-n-1, im.channels()); // Initialize next carving

        for (int w = 0; w < im.width(); w++) {          // Iterate over all columns
            int j = 0;
            for (int h = 0; h < im.height()-n; h++) { // Iterate over current height
                if (h != seam.at(current.width()-w-1)) {  // Check that current index is not seam index
                    current(w, j, 0) = carving.at(n)(w, h, 0);
                    current(w, j, 1) = carving.at(n)(w, h, 1);
                    current(w, j, 2) = carving.at(n)(w, h, 2);
                    j += 1;
                }
            }
        }
        carving.push_back(current); // Add in-progress carved image to vector to call next iteration.
    }
    return carving.at(N); // Return final carved image, carved N number of times.
}


Image object_removal_seam_carving(const Image &im, vector<int> remove_region, bool vertical) {
    // Remove a rectangular region from an image using seam carving. To perform
    // the removal, the function calculates the L1 energy then sets the energy of
    // the region to negative 1000. This guarantees when the cumulative energy
    // map is made that the minimum seam will always pass through the region.
    // The function then recurses the needed number of times to remove all pixels
    // within the designated region.
    // Input: Three-channel image, vector of coordinates in (X1, X2, Y1, Y2 format),
    //        and a boolean that is True if vertical carving is desired, false if horizontal.
    // Output: Three-channel image with region removed.

    if      (remove_region.at(0) > remove_region.at(1)) {return im;} // Base case for vertical carving.
    else if (remove_region.at(2) > remove_region.at(3)) {return im;} // Base case for horizontal carving.
    else {

        Image l1 = L1_energy(im);                                          // Calculate L1 energy
        for (int x = remove_region.at(0); x <= remove_region.at(1); x++) {
            for (int y = remove_region.at(2); y <= remove_region.at(3); y++) {
                l1(x, y, 0) = -1000.0f;                                    // Set region to -1000 so seams always go there
            }
        }

        if (vertical) {                                 // Vertical seam carving case
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
            
            vector<int> vert_seam = vert_minimum_seam(cem); // Get vertical seam

            Image current(im.width()-1, im.height(), im.channels()); // Make output image to add carving too

            for (int h = 0; h < im.height(); h++) {        // Iterate over all rows
                int i = 0;
                for (int w = 0; w < im.width(); w++) { // Iterate over current width
                    if (w != vert_seam.at(im.height()-1-h)) {             // Check that current index is not seam index
                        current(i, h, 0) = im(w, h, 0);
                        current(i, h, 1) = im(w, h, 1);
                        current(i, h, 2) = im(w, h, 2);
                        i += 1;
                    }
                }
            }
            vector<int> new_region = {remove_region.at(0),  // Adjust removal region for recursive call
                                      remove_region.at(1)-1, 
                                      remove_region.at(2), 
                                      remove_region.at(3)
            };
            return object_removal_seam_carving(current, new_region, true); // Recurse
        }

        else {                                           // Horizontal seam carving case
            Image cem(im.width(), im.height(), 1);       // Initialize output image
            for (int y = 0; y < im.height(); y++) {      // Copy left row of input energy map
                cem(0, y) = l1(0, y);
            }
            for (int w = 1; w < im.width(); w++) {       // Iterate over column [1,...,n]
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

            vector<int> hori_seam = hori_minimum_seam(cem); // Get horizontal seam

            Image current(im.width(), im.height()-1, im.channels()); // Make output image to add carving too

            for (int w = 0; w < im.width(); w++) {          // Iterate over all columns
                int j = 0;
                for (int h = 0; h < im.height()-1; h++) { // Iterate over current height
                    if (h != hori_seam.at(im.width()-1-w)) {              // Check that current index is not seam index
                        current(w, j, 0) = im(w, h, 0);
                        current(w, j, 1) = im(w, h, 1);
                        current(w, j, 2) = im(w, h, 2);
                        j += 1;
                    }
                }
            }
            vector<int> new_region = {remove_region.at(0),  // Add removal region for next recursive call
                                      remove_region.at(1), 
                                      remove_region.at(2), 
                                      remove_region.at(3)-1
            };
            return object_removal_seam_carving(current, new_region, false); // Recurse
        }
    }
}


Image optimal_order_seam_carving(const Image &im, int new_width, int new_height) {
    // Seam carve an image to a target dimension of (new_width x new_height)
    // using an algorithm for optimum energy seam carving by alternating vertical
    // and horizontal carves based on the optimal sequence of carves.
    // NOTE: Assumes new_width < im.width() and new_height < im.height().
    // Input: Three-channel input image and target width and height of output image.
    // Output: Input image seam carved optimally to the target dimensions.

    // INCOMPLETE!!! NO TEST CASE AND UNFINISHED.

    if (new_width == im.width()) {
        return horizontal_seam_carving(im, im.height() - new_height); // Case where width stays same
    }

    else if (new_height == im.height()) {
        return vertical_seam_carving(im, im.width() - new_width);     // Case where height stays same
    }

    else {
        int r = im.height() - new_height; // Get r and c values to use for formula
        int c = im.width() - new_width;
    }   
}