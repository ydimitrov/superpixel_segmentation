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
#include <vector>
#include <fstream>
#include <string>
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp> 

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

typedef unsigned char uchar;
typedef struct { uchar r, g, b; } rgb;

std::vector<int> average_colour(std::vector<std::vector<int>> children, image<rgb> *im) {
    
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

void colour_clusters_with_avg_colour(image<rgb> *output, std::vector<int> colour, std::vector<std::vector<int>> children) {
    
    rgb avg_colour = {(uchar) colour[0], (uchar) colour[1], (uchar) colour[2]};

    for (int row = 0; row < children.size(); row++) 
        imRef(output, children[row][1], children[row][0]) = avg_colour; 
}

cv::Mat construct_hist_hsv (const cv::Mat &src_img) {
    
    cv::Mat hist, hsv_base;

    cvtColor(src_img, hsv_base, cv::COLOR_BGR2HSV);

    // int h_bins = 50; int s_bins = 60;
    int h_bins = 180; int s_bins = 256;
    int histSize[] = {h_bins, s_bins};

    // hue varies from 0 to 179, saturation from 0 to 255
    float h_ranges[] = {0, 180};
    float s_ranges[] = {0, 256};

    const float* ranges[] = {h_ranges, s_ranges};

    // Use the o-th and 1-st channels
    int channels[] = {0, 1};

    calcHist(&hsv_base, 1, channels, cv::Mat(), hist, 1, histSize, ranges, true, false);
    normalize(hist, hist, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

    return hist;
}

cv::Mat construct_hist_rgb (const cv::Mat &src_img) {
    
    cv::Mat hist;

    const int sizes[] = {256, 256, 256};
    
    float rRange[] = {0, 256};
    float gRange[] = {0, 256};
    float bRange[] = {0, 256};
    
    const float *ranges[] = {rRange, gRange, bRange};

    int channels[] = {0, 1, 2};    
    
    calcHist(&src_img, 1, channels, cv::Mat(), hist, 1, sizes, ranges, true, false);
    normalize(hist, hist, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

    return hist;
}


cv::Mat convert_component_to_Mat(std::vector<std::vector<int>> children, image<rgb> *input) {
   
    cv::Mat Mat_obj = cv::Mat(children.size(), 1, CV_8UC3); // dimension is #number of pixels x 1
    
    for(int row = 0; row < Mat_obj.rows; row++) {
        Mat_obj.at<cv::Vec3b>(row, 0)[0] = imRef(input, children[row][1], children[row][0]).b; //cv::vec3b
        Mat_obj.at<cv::Vec3b>(row, 0)[1] = imRef(input, children[row][1], children[row][0]).g; //cv::vec3b
        Mat_obj.at<cv::Vec3b>(row, 0)[2] = imRef(input, children[row][1], children[row][0]).r; //cv::vec3b
    }

    return Mat_obj;
}

cv::Mat convert_sample_to_nonzero_Mat(const cv::Mat &image) {

    int index = 0, count = 0;

    for (int row = 0; row < image.rows; row++) {
        for (int col = 0; col < image.cols; col++) {
            if (image.at<cv::Vec3b>(row, col)[0] != 0 && image.at<cv::Vec3b>(row, col)[1] != 0 && image.at<cv::Vec3b>(row, col)[2] != 0)
                count++;
        }
    }

    cv::Mat Mat_obj = cv::Mat::zeros(count, 1, CV_8UC3);

    for (int row = 0; row < image.rows; row++) {
        for (int col = 0; col < image.cols; col++) {
            if (image.at<cv::Vec3b>(row, col)[0] != 0 && image.at<cv::Vec3b>(row, col)[1] != 0 && image.at<cv::Vec3b>(row, col)[2] != 0) { //cv::vec3b
                Mat_obj.at<cv::Vec3b>(index, 0)[0] = image.at<cv::Vec3b>(row, col)[0];
                Mat_obj.at<cv::Vec3b>(index, 0)[1] = image.at<cv::Vec3b>(row, col)[1];
                Mat_obj.at<cv::Vec3b>(index, 0)[2] = image.at<cv::Vec3b>(row, col)[2];
                index++;
            }
        }
    }

    return Mat_obj;
}

void print_Mat(const cv::Mat &obj) {

    std::cout << "histogram type = " << obj.type() << std::endl;
    // std::cout << "histogram depth = " << obj.depth() << std::endl;
    std::cout << "histogram size = " << obj.total() << std::endl;
    // std::cout << "histogram channels = " << obj.channels() << std::endl;

    // std::cout << "histogram rows = " << obj.rows << std::endl;
    // std::cout << "histogram cols = " << obj.cols << std::endl;

    // for (int row = 0; row < obj.rows; row++) {
    //     for (int col = 0; col < obj.cols; col++) { 
    //         std::cout << obj.at<float>(row, col) << " ";
    //     }
    // }

    // std::cout << std::endl;
}

void write_csv(std::string filename, const cv::Mat &obj) {

   std::ofstream myfile;
   myfile.open(filename.c_str());
   myfile << cv::format(obj, "numpy") << std::endl;
   myfile.close();
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
