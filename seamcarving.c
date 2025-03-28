#include "seamcarving.h"
#include <stdio.h>
#include <math.h>

void calc_energy(struct rgb_img *im, struct rgb_img **grad) {

    int rx = right minus left;
    int gx = ;
    int bx = ;
    int ry = top minus bottom;
    int gy = ;
    int by = ;

    int xvals = im->width - 1; // make this 1 less than the i-th
    int yvals = im->height - 1; // make this 1 less than the i-th
    int xgrad = 0;
    int ygrad = 0;

    double energy = 0;

    for (int i = 1; i < xvals; i++) { // start at 1 and ends before last pixel so that we dont do edges
        for (int j = 1; j < yvals; j++) {
            // DO SOME PSEUDOCODE PLANNING ABOUT HOW TO ACCESS EACH FOUR SIDES (UP DOWN LEFT RIGHT)
            // AND HOW TO CALCULATE THE CHANGE LIKE WHAT DO YOU WANNA ACTUALLY DO

            ry1, rx1 = get_pixel(im, j-1, i-1, 0); // check that its returning ry, rx not rx, ry (actually it might be...)
            gy1, gx1 = get_pixel(im, j-1, i-1, 1);
            gy1, bx1 = get_pixel(im, j-1, i-1, 2);

            ry2, rx2 = get_pixel(im, j, i, 0); // check that its returning ry, rx not rx, ry (actually it might be...)
            gy2, gx2 = get_pixel(im, j, i, 1);
            gy2, bx2 = get_pixel(im, j, i, 2);

            xgrad = (rx) * (rx) + (gx) * (gx) + (bx) * (bx);
            ygrad = (ry) * (ry) + (gy) * (gy) + (by) * (by);

            energy = sqrt(xgrad * xgrad + ygrad * ygrad);
        }
    }

    *grad = ; // put smth in it
}