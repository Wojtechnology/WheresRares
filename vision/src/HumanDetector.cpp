#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

class HumanDetector {
public:
  static std::vector<Rect> detect(Mat image_to_detect) {
    // TODO(tullie): Detect humans.
    return std::vector<Rect>();
  }
};

int main(int argc, char** argv )
{
    if (argc != 2) {
        printf("Usage: %s", argv[0]);
        return -1;
    }

    Mat image_to_detect;
    image_to_detect = imread(argv[1], 1);

    std::vector<Rect> human_rects = HumanDetector::detect(image_to_detect);

    for (Rect human_rect : human_rects) {
      rectangle(image_to_detect, human_rect, Scalar(0, 255, 0));
    }

    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", image_to_detect);
    waitKey(0);

    return 0;
}
