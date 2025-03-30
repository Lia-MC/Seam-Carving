#include "seamcarving.h"
#include <stdio.h>
#include <math.h>

void calc_energy(struct rgb_img *im, struct rgb_img **grad) {

    int rx;
    int gx;
    int bx;
    int ry;
    int gy;
    int by;

    int xvals = im->width; // handled wrapping in for loop
    int yvals = im->height; // handled wrapping in for loop
    int xgrad = 0;
    int ygrad = 0;

    double energy = 0;
    uint8_t dualgradientenergy;

    create_img(grad, im->height, im->width);

    for (int i = 0; i < xvals; i++) { // start at 1 and ends before last pixel so that we dont do edges
        for (int j = 0; j < yvals; j++) {

            ry = get_pixel(im, (j + 1) % im->height, i, 0) - get_pixel(im, (j - 1 + im->height) % im->height, i, 0); 
            gy = get_pixel(im, (j + 1) % im->height, i, 1) - get_pixel(im, (j - 1 + im->height) % im->height, i, 1); 
            by = get_pixel(im, (j + 1) % im->height, i, 2) - get_pixel(im, (j - 1 + im->height) % im->height, i, 2); 

            rx = get_pixel(im, j, (i + 1) % im->width, 0) - get_pixel(im, j, (i - 1 + im->width) % im->width, 0); 
            gx = get_pixel(im, j, (i + 1) % im->width, 1) - get_pixel(im, j, (i - 1 + im->width) % im->width, 1);
            bx = get_pixel(im, j, (i + 1) % im->width, 2) - get_pixel(im, j, (i - 1 + im->width) % im->width, 2);

            xgrad = (rx * rx) + (gx * gx) + (bx * bx);
            ygrad = (ry * ry) + (gy * gy) + (by * by);

            energy = sqrt(xgrad + ygrad);

            dualgradientenergy = (uint8_t) (energy / 10);

            // For each pixel, set the r, g, and b channels to the same value 
            // (the energy divided by 10 and cast to uint8_t).
            set_pixel(*grad, j, i, dualgradientenergy, dualgradientenergy, dualgradientenergy);
        }
    }
}

// int main() {
//     struct rgb_img *im;

//     read_in_img(&im, "3x4.png");
    
//     struct rgb_img *grad;

//     create_img(&grad, im->height, im->width);
  
//     calc_energy(im,  &grad);
  
//     print_grad(grad);

//     return 0;
// }