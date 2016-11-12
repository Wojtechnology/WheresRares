#include <stdio.h>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

class BackgroundSubtractor {
public:
  static bool mask(std::string video_file) {
    Mat frame;
    Mat resizeF;
    Mat fgMaskMOG;

    Ptr<BackgroundSubtractor> pMOG = new BackgroundSubtractorMOG();
    VideoCapture video(video_file);

    if(!video.isOpened()) {
      return false;
    }

    namedWindow("Masked", 1);
    while(1) {
      if (!video.read(frame)) {
        break;
      }

      Mat cameraFrame;
      resize(frame, resizeF, Size(frame.size().width / 4, frame.size().height / 4))
      pMOG->operator()(resizeF, fgMaskMOG);
    }

    imshow("Origin", resizeF);
    imshow("MOG", fgMaskMOG);

    if (waitKey(30) >= 0) {
      break;
    }

    return true;
  }
};


int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Usage: %s <detection_video>", argv[0]);
    return -1;
  }

  BackgroundSubtractor::mask(argv[1]);
  return 0;
}
