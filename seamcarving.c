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

            set_pixel(*grad, j, i, dualgradientenergy, dualgradientenergy, dualgradientenergy);
        }
    }
}

void dynamic_seam(struct rgb_img *grad, double **best_arr) {
    for (int i = 0; i < grad->width; i++) {
        for (int j = 0; j < grad->height; j++) {
            *best_arr = calc_energy(im, &grad) + min()
        }
    }
    // allocates and computes the dynamic array *best_arr.
    // (*best_arr)[i*width+j] contains the minimum cost of a seam from the top of grad to the point (i,j)

    // pseudocode:
    /*
    go through the 0,0 to point i j 
    at each point, calculate the minimum cost of the seam and store it in best_arr
    to calculate minimum cost of seam up until point (i, j):
     - do the thing where you start at (i, j) and try all the ways of going backwards to the top row and 
       whatever path has the lowest one is the minimum one

       1. look at the gradient matrix
       2. start at top row, look at each elem from left to right, and then move to the next row
       3. for each elem, compare the diagleft, directabove, diagright elems 
       and for each of those elems, find the path that takes the lowest energy 
       (essentially keep checking those 3 for each path until top is reached aka row = 0)
       4. you better handle edge cases (at the edges, cant check certain diags) -> if statement
       5. keep note of the path - how? or not... might not be needed
       6. for each path, compare it with the prexisting energy value and compare, and if
       it's lower, then make the new energy as the minimum. note: initialize it to infinity pls
       
     */


    // q1
    // Part 2 of Project 2 says that (*best_arr)[i*width+j] contains the minimum cost of a seam 
    // from the top of grad to the point (i, j). Should we assume that i and j start at 0 or 1? 
    // (for example, is the pixel in the very top left-hand corner at index (0, 0), or (1, 1)? 
    // Or does i start at 1 while j starts at 0, making the left top pixel (1, 0)?)

    // I had initially assumed that both i and j started at 0, but this results in two points 
    // occasionally sharing the same i*width+j value, which causes problems. 

    // They start with 0.
    // i*width+j is unique for a particular (i, j) as long as j < width, 
    // which it should be since j goes from 0 to width-1.


    // q2 

    // When implementing the dynamic_seam function, should we only consider paths that 
    // go downwards (i.e., straight down, diagonally down one and to the left or diagonally 
    // down one and to the right), or should we consider purely horizontal paths as well 
    // (i.e., just to the left or just to the right)

    // Only downwards

}

void recover_path(double *best, int height, int width, int **path) {
    //
}

void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path) {
    //
}