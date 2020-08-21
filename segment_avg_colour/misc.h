/*
Copyright (C) 2006 Pedro Felzenszwalb

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*/

/* random stuff */

#ifndef MISC_H
#define MISC_H

#include <cmath>
#include <iostream>
#include <tgmath.h>
#include <math.h>
#include <vector>
#include <random>

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

typedef unsigned char uchar;
typedef struct { uchar r, g, b; } rgb;

std::vector<int> average_colour(std::vector<std::vector<int>> children, image<rgb> *im){
    
    std::vector<int> avg_colour;
    int total_r = 0, total_g = 0, total_b = 0;
    
    for (int row = 0; row < children.size(); row++) {
        total_r += imRef(im, children[row][1], children[row][0]).r;
        total_g += imRef(im, children[row][1], children[row][0]).g;
        total_b += imRef(im, children[row][1], children[row][0]).b;
    }

    avg_colour.push_back((int) (total_r / children.size())); // average red colour of all children of cluster
    avg_colour.push_back((int) (total_g / children.size())); // average green colour of all children of cluster
    avg_colour.push_back((int) (total_b / children.size())); // average blue colour of all children of cluster

    return avg_colour;
}

bool allowed_range(std::vector<int> sample, std::vector<int> calculated_average, int range) {
    
    int max_r, max_g, max_b;
    int min_r, min_g, min_b;

    max_r = sample[0] + range;
    max_g = sample[1] + range;
    max_b = sample[2] + range;

    min_r = sample[0] - range;
    min_g = sample[1] - range;
    min_b = sample[2] - range;

    if(calculated_average[0] < min_r || calculated_average[0] > max_r)
        return false;
    if(calculated_average[1] < min_g || calculated_average[1] > max_g)
        return false;
    if(calculated_average[2] < min_b || calculated_average[2] > max_b)
        return false;

    return true;
}

bool allowed_range_euclidean(std::vector<int> sample, std::vector<int> calculated_average, int range) {

    double euclidean_distance = sqrt(pow((sample[0] - calculated_average[0]), 2) + pow((sample[1] - calculated_average[1]), 2) + pow((sample[2] - calculated_average[2]), 2));
    return (euclidean_distance <= range) ? true : false;
}

void colour_clusters_with_avg_colour(image<rgb> *output, std::vector<int> colour, std::vector<std::vector<int>> children) {
    
    rgb avg_colour = {(uchar) colour[0], (uchar) colour[1], (uchar) colour[2]};

    for (int row = 0; row < children.size(); row++) 
        imRef(output, children[row][1], children[row][0]) = avg_colour; 
}

inline bool operator==(const rgb &a, const rgb &b) {
  return ((a.r == b.r) && (a.g == b.g) && (a.b == b.b));
}

template <class T>
inline T abs(const T &x) { return (x > 0 ? x : -x); };

template <class T>
inline int sign(const T &x) { return (x >= 0 ? 1 : -1); };

template <class T>
inline T square(const T &x) { return x*x; };

template <class T>
inline T bound(const T &x, const T &min, const T &max) {
  return (x < min ? min : (x > max ? max : x));
}

template <class T>
inline bool check_bound(const T &x, const T&min, const T &max) {
  return ((x < min) || (x > max));
}

inline int vlib_round(float x) { return (int)(x + 0.5F); }

inline int vlib_round(double x) { return (int)(x + 0.5); }

inline double gaussian(double val, double sigma) {
  return exp(-square(val/sigma)/2)/(sqrt(2*M_PI)*sigma);
}

#endif
