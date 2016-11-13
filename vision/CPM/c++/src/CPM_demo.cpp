#include "PoseEstimator.hpp"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace caffe;
using namespace cv;

int main(int argc, char** argv) {
  if (argc != 4) {
    printf("Usage %s <model_file> <trained_file> <test_image>", argv[0]);
    return -1;
  }

  std::string test_image_file = argv[3];
  Mat test_image = imread(test_image_file, IMREAD_UNCHANGED);

  PoseEstimator pose_estimator;
  vector<Point> limbs = pose_estimator.detectLimbs(test_image);
  for (Point limb: limbs) {
    circle(test_image, limb, 2, Scalar(0, 255, 0), 3);
  }

  namedWindow("Display View", CV_WINDOW_AUTOSIZE);
  imshow("Display View", test_image);
  waitKey(0);
}
