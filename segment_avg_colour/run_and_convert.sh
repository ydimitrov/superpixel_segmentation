#!/bin/bash
name=${4%.*} 
./segment $1 $2 $3 $4 ${name}_sig$1_k$2_min$3.ppm $5
# python3 ppm_to_png_single.py ${name}_sig$1_k$2_min$3.ppm
# rm ${name}_sig$1_k$2_min$3.ppm
mv coloured_clusters.ppm segmentation.ppm ${name}_sig$1_k$2_min$3.ppm converted_images/
cd converted_images/
./run.sh
