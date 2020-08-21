Implementation of the segmentation algorithm described in:

Efficient Graph-Based Image Segmentation
Pedro F. Felzenszwalb and Daniel P. Huttenlocher
International Journal of Computer Vision, 59(2) September 2004.

The program takes a color image (PPM format) and produces a segmentation
with a random color assigned to each region.

1) Type "make" to compile "segment".

2) Run "segment sigma k min input output".

The parameters are: (see the paper for details)

sigma: Used to smooth the input image before segmenting it.
k: Value for the threshold function.
min: Minimum component size enforced by post-processing.
input: Input image.
output: Output image.

Typical parameters are sigma = 0.5, k = 500, min = 20.
Larger values for k result in larger components in the result.

!!! To run algorithm after modifications run the following command:

./run_and_convert.sh 0 0.002 500 square_ppms/1.ppm 40

- square_ppms/ contains all test images converted in .ppm format

- last argument sets the allowed range within which a given colour is considered similar to the average

- resulting segmentation and clustering are stored in converted_images/

- it is important to set the average colour of the sample in file segment-image.h on lines 174-181 to the corresponding image