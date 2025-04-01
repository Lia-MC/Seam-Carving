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
    uint8_t optimal;

    // allocate memory for best_arr (enough for a lot of pixels so that i dont have to realloc)
    // or check realloc when storing the value in best arr each time works too and is probably better

    *best_arr = (double *) malloc(grad->height * grad->width * sizeof(double)); 

    for (int y = 0; y < grad->height; y++) {
        for (int x = 0; x < grad->width; x++) {
            if (y == 0) {
                // just store the current value, dont do any left or right checking
                // energy is alr stored in grad so we can just get the pixel instead of recalculating the gradient lol
                optimal = get_pixel(grad, y, x, 0);
            }
            // note: everything in row above for bestarr is alr calculated by the time we want that info
            else if (x == 0) {
                // no left, just right and center
                optimal = get_pixel(grad, y, x, 0) + fmin(((*best_arr)[(y-1) * grad->width + x]), ((*best_arr)[(y-1) * grad->width + x+1]));
            }
            else if (x == grad->height - 1) {
                // no right, just left and center
                optimal = get_pixel(grad, y, x, 0) + fmin(((*best_arr)[(y-1) * grad->width + x-1]), ((*best_arr)[(y-1) * grad->width + x]));
            } else {
                optimal = get_pixel(grad, y, x, 0) + fmin(((*best_arr)[(y-1) * grad->width + x-1]), fmin(((*best_arr)[(y-1) * grad->width + x]), ((*best_arr)[(y-1) * grad->width + x+1])));
            }

            // realloc no longer needed because grad->height and grad->width covers it
            (*best_arr)[y*(grad->width)+x] = optimal;
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
       7. store the minimum value in the best_array, right after the last element
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
    // start at top row element 0
    // for each possibility of the next element (consider edge cases - sides, if it reaches bottom), 
    // recursively? calculate the minimum path

    // This function allocates a path through the minimum seam as defined by the array best.
    // For the best array above, the path is [3, 4, 3, 2, 2].

    int min_index = 0;

    *path = malloc(height * sizeof(int));

    for (int x = 1; x < width; x++) {
        if (best[(height - 1) * width + x] < best[(height - 1) * width + min_index]) {
            min_index = x;
        }
    }

    (*path)[height - 1] = min_index;

    for (int y = height - 2; y >= 0; y--) {
        int prev_x = (*path)[y + 1];  

        int best_x = prev_x;
        if (prev_x > 0 && best[y * width + prev_x - 1] < best[y * width + best_x]) {
            best_x = prev_x - 1;
        }
        if (prev_x < width - 1 && best[y * width + prev_x + 1] < best[y * width + best_x]) {
            best_x = prev_x + 1;
        }

        (*path)[y] = best_x; 
    }
}

void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path) {
    // The function creates the destination image, and writes to it the source image, with the seam removed.

    int foundinrow = 0; // starts false

    uint8_t r;
    uint8_t g;
    uint8_t b;

    create_img(dest, src->height, src->width - 1);

    for (int y = 0; y < src->height; y++) {
        foundinrow = 0;
        for (int x = 0; x < src->width; x++) {
            if (x == path[y]) {
                foundinrow = 1; // skip this col in the reading for that row
            }
            r = get_pixel(src, y, x + foundinrow, 0);
            g = get_pixel(src, y, x + foundinrow, 1);
            b = get_pixel(src, y, x + foundinrow, 2);
            set_pixel(*dest, y, x, r, g, b);
        }
    }
}