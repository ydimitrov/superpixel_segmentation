# Superpixel segmentation for visual inspection
Implementation of the segmentation algorithm described in:

Efficient Graph-Based Image Segmentation
Pedro F. Felzenszwalb and Daniel P. Huttenlocher
International Journal of Computer Vision, 59(2) September 2004.

The program takes a color image (PPM format) and produces a segmentation with a the average colour for each cluster. The algorithm analyses the histogram or average colour of a cluster. If the average colour or histogram match with the provided example, the cluster is included for segmentation.

### Prerequisites
- C++11
- Python 3.5.2

### Parameters

- sigma: Used to smooth the input image before segmenting it

- k: Value for the threshold function

- min: Minimum component size enforced by post-processing

- input: Input image

- output: Output image

Typical parameters are sigma = 0.5, k = 500, min = 20.
Larger values for k result in larger components in the result. See the paper for details

## Getting Started
To run pure algorithm:

1) Type "make" to compile "segment".

2) Run 
```
./segment sigma k min input output
```

To run algorithm after modifications run the following command:
```
./run_and_convert.sh 0 0.002 500 square_ppms/1.ppm 40
```
- square_ppms/ contains all test images converted in .ppm format

- last argument sets the allowed range within which a given colour is considered similar to the average



- resulting segmentation and clustering are stored in converted_images/

- it is important to set the average colour of the sample in file segment-image.h on lines 174-181 to the corresponding image
## Example segmentations

![example1](https://github.com/ydimitrov/superpixel_segmentation/examples/master/example1.jpg?raw=true)

![example2](https://github.com/ydimitrov/superpixel_segmentation/examples/master/example2.jpg?raw=true)

![example3](https://github.com/ydimitrov/superpixel_segmentation/examples/example3.jpg?raw=true)

## Authors
Yordan Dimitrov under the supervision of Thomas Woudsma and Tony Paulussen at Prodrive Technologies
