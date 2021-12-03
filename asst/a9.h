#ifndef A9_H_PHUDVTKB
#define A9_H_PHUDVTKB

// Write your declarations here, or extend the Makefile if you add source
// files
Image gradientX(const Image &im, bool clamp);

Image gradientY(const Image &im, bool clamp);

Image L1_energy(const Image &im);

Image seam_carving(const Image &im);


#endif /* end of include guard: A9_H_PHUDVTKB */
