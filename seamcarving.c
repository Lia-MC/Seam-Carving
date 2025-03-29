#include "seamcarving.h"
#include <stdio.h>
#include <math.h>

void calc_energy(struct rgb_img *im, struct rgb_img **grad) {

    int rx1;
    int gx1;
    int bx1;
    int ry1;
    int gy1;
    int by1;

    int rx2;
    int gx2;
    int bx2;
    int ry2;
    int gy2;
    int by2;

    int xvals = im->width - 1; // make this 1 less than the i-th
    int yvals = im->height - 1; // make this 1 less than the i-th
    int xgrad = 0;
    int ygrad = 0;

    double energy = 0;
    uint8_t dualgradientenergy;

    for (int i = 1; i < xvals; i++) { // start at 1 and ends before last pixel so that we dont do edges
        for (int j = 1; j < yvals; j++) {

            ry1, rx1 = get_pixel(im, j+1, i, 0) - get_pixel(im, j-1, i, 0); 
            gy1, gx1 = get_pixel(im, j+1, i, 0) - get_pixel(im, j-1, i, 1);
            gy1, bx1 = get_pixel(im, j+1, i, 0) - get_pixel(im, j-1, i, 2);

            ry2, rx2 = get_pixel(im, j, i+1, 0) - get_pixel(im, j, i-1, 0); 
            gy2, gx2 = get_pixel(im, j, i+1, 0) - get_pixel(im, j, i-1, 1);
            gy2, bx2 = get_pixel(im, j, i+1, 0) - get_pixel(im, j, i-1, 2);

            xgrad = ((ry1) * (ry1), (rx1) * (rx1)) + ((gy1) * (gy1), (gx1) * (gx1)) + ((by1) * (by1), (bx1) * (bx1));
            ygrad = ((ry2) * (ry2), (rx2) * (rx2)) + ((gy2) * (gy2), (gx2) * (gx2)) + ((by2) * (by2), (bx2) * (bx2));

            energy = sqrt(xgrad * xgrad + ygrad * ygrad);

            energy *= 0.1;

            dualgradientenergy = (uint8_t) energy;

            // For each pixel, set the r, g, and b channels to the same value 
            // (the energy divided by 10 and cast to uint8_t).
            set_pixel(*grad, j, i, dualgradientenergy, dualgradientenergy, dualgradientenergy);
        }
    }
}