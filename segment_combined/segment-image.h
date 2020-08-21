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

#ifndef SEGMENT_IMAGE
#define SEGMENT_IMAGE

#include <cstdlib>
#include <image.h>
#include <misc.h>
#include <filter.h>
#include "pnmfile.h"
#include "segment-graph.h"
#include <unordered_set>
#include <iostream>
#include <string>
#include <vector>
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/imgproc/imgproc.hpp>


// random color
rgb random_rgb(){ 
  rgb c;
  double r;
  
  c.r = (uchar)random();
  c.g = (uchar)random();
  c.b = (uchar)random();

  return c;
}

// dissimilarity measure between pixels
static inline float diff(image<float> *r, image<float> *g, image<float> *b,
			 int x1, int y1, int x2, int y2) {
  return sqrt(square(imRef(r, x1, y1)-imRef(r, x2, y2)) +
	      square(imRef(g, x1, y1)-imRef(g, x2, y2)) +
	      square(imRef(b, x1, y1)-imRef(b, x2, y2)));
}

/*
 * Segment an image
 *
 * Returns a color image representing the segmentation.
 *
 * im: image to segment.
 * sigma: to smooth the image.
 * c: constant for treshold function.
 * min_size: minimum component size (enforced by post-processing stage).
 * num_ccs: number of connected components in the segmentation.
 */
image<rgb> *segment_image(image<rgb> *im, float sigma, float c, int min_size,
			  int *num_ccs, const cv::Mat &sample_img, float threshold) {
  int width = im->width();
  int height = im->height();

  image<float> *r = new image<float>(width, height);
  image<float> *g = new image<float>(width, height);
  image<float> *b = new image<float>(width, height);

  // smooth each color channel  
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      imRef(r, x, y) = imRef(im, x, y).r;
      imRef(g, x, y) = imRef(im, x, y).g;
      imRef(b, x, y) = imRef(im, x, y).b;
    }
  }
  image<float> *smooth_r = smooth(r, sigma);
  image<float> *smooth_g = smooth(g, sigma);
  image<float> *smooth_b = smooth(b, sigma);
  delete r;
  delete g;
  delete b;
 
  // build graph
  edge *edges = new edge[width*height*4];
  int num = 0;
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      
      if (x < width-1) {
      	edges[num].a = y * width + x;
      	edges[num].b = y * width + (x+1);
      	edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, x+1, y);
      	num++;
      }

      if (y < height-1) {
      	edges[num].a = y * width + x;
      	edges[num].b = (y+1) * width + x;
      	edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, x, y+1);
      	num++;
      }

      if ((x < width-1) && (y < height-1)) {
      	edges[num].a = y * width + x;
      	edges[num].b = (y+1) * width + (x+1);
      	edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, x+1, y+1);
      	num++;
      }

      if ((x < width-1) && (y > 0)) {
      	edges[num].a = y * width + x;
      	edges[num].b = (y-1) * width + (x+1);
      	edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, x+1, y-1);
      	num++;
      }
    }
  }
  delete smooth_r;
  delete smooth_g;
  delete smooth_b;

  // segment
  universe *u = segment_graph(width*height, num, edges, c);
  
  // post process small components
  for (int i = 0; i < num; i++) {
    int a = u->find(edges[i].a);
    int b = u->find(edges[i].b);
    if ((a != b) && ((u->size(a) < min_size) || (u->size(b) < min_size)))
      u->join(a, b);
  }
  delete [] edges;
  *num_ccs = u->num_sets();


  // pick random colors for each component
  rgb *colors = new rgb[width*height];
  for (int i = 0; i < width*height; i++)
    colors[i] = random_rgb();

  rgb white = {255, 255, 255};
  std::unordered_set<int> final_components;

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int comp = u->find(y * width + x);
      if (final_components.find(comp) == final_components.end())
        final_components.insert(comp);
    }
  }
  
  for(auto itr = final_components.begin(); itr != final_components.end(); itr++)
    std::cout << *itr << ", ";
  std::cout << std::endl;

  image<rgb> *output = new image<rgb>(width, height);
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int comp = u->find(y * width + x);
      imRef(output, x, y) = colors[comp];
    }
  }
  // savePPM(output, "output.ppm");

  std::cout << "final_components = " << final_components.size() << std::endl;

  image<rgb> *coloured_clusters = new image<rgb>(width, height);
  std::vector<int> segmentation_components;

  cv::Mat sample_img_hist = construct_hist(convert_sample_to_nonzero_Mat(sample_img));
  write_csv("sample_histogram.py", sample_img_hist);
    // std::vector<int> sample_average = {77, 192, 153};    // 1.ppm
  std::vector<int> sample_average = {209, 225, 193};   // 2_cropped.ppm
  // std::vector<int> sample_average = {129, 201, 188};   // 16.ppm
  // std::vector<int> sample_average = {223, 236, 220};   // 12.ppm
  // std::vector<int> sample_average = {164, 243, 228};   // 4.ppm
  // std::vector<int> sample_average = {176, 185, 161};     //9_2_cropped.ppm
  // std::vector<int> sample_average = {230, 245, 232};   // 13.ppm
  // std::vector<int> sample_average = {63, 185, 161};   // test.ppm
  // std::vector<int> sample_average = {67, 185, 158};   // test2.ppm
  // std::vector<int> sample_average = {171, 181, 156};     //9_cropped.ppm
  // std::vector<int> sample_average = {235, 246, 236};     //18.ppm
  // std::vector<int> sample_average = {255, 255, 255};     //11.ppm
  // std::vector<int> sample_average = {197, 218, 194};     //14.ppm
  // std::vector<int> sample_average = {189, 213, 193};     //15.ppm

  for(auto itr = final_components.begin(); itr != final_components.end(); itr++) {
    int component = *itr;
    std::vector<std::vector<int>> children = u->get_children(component, width, height); // this stays
    std::vector<int> avg_colour = average_colour(children, im);
    cv::Mat component_Mat = convert_component_to_Mat(children, im);
    cv::Mat component_hist = construct_hist(component_Mat);
    colour_clusters_with_avg_colour(coloured_clusters, avg_colour, children);
    // if(compareHist(sample_img_hist, component_hist, CV_COMP_CHISQR) <= threshold)
    // if(compareHist(sample_img_hist, component_hist, CV_COMP_INTERSECT) >= threshold)
    // if(compareHist(sample_img_hist, component_hist, CV_COMP_BHATTACHARYYA) <= threshold)
    if(compareHist(sample_img_hist, component_hist, CV_COMP_CORREL) * (1 / euclidean_distance(sample_average, avg_colour)) >= threshold)
    // if(compareHist(sample_img_hist, component_hist, CV_COMP_CORREL) >= threshold)
      segmentation_components.push_back(component);
  }
  savePPM(coloured_clusters, "coloured_clusters.ppm");
  
  image<rgb> *segmentation = new image<rgb>(width, height);
  std::cout << "components to segment = " << segmentation_components.size() << std::endl;
  for(auto itr = segmentation_components.begin(); itr != segmentation_components.end(); itr++){
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        int comp = u->find(y * width + x);
      	if(comp == *itr){
          imRef(segmentation, x, y) = white;
        }
      }
    }
  }
  savePPM(segmentation, "segmentation.ppm");  

  for(auto itr = segmentation_components.begin(); itr != segmentation_components.end(); itr++)
    std::cout << *itr << ", ";
  std::cout << std::endl;

  delete [] colors;  
  delete u;

  return output;
}

#endif
