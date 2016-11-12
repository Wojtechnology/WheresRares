#include <stdio.h>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

class HumanDetector {
 public:
  static std::vector<Rect> detect(Mat image_to_detect) {
    cv::HOGDescriptor hog;
    hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());

    std::vector<Rect> found_humans;
    hog.detectMultiScale(image_to_detect, found_humans, 0, cv::Size(8, 8),
                         cv::Size(32, 32), 1.05, 2);  // Default parameters.

    return std::vector<Rect>();
  }
};

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Usage: %s <detection_image>", argv[0]);
    return -1;
  }

  Mat image_to_detect;
  image_to_detect = imread(argv[1], 1);

  std::vector<Rect> human_rects = HumanDetector::detect(image_to_detect);

  for (Rect human_rect : human_rects) {
    rectangle(image_to_detect, human_rect, Scalar(0, 255, 0));
  }

  namedWindow("Display Image", WINDOW_AUTOSIZE);
  imshow("Display Image", image_to_detect);
  waitKey(0);

  return 0;
}
