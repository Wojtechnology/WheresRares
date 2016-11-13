#include <stdio.h>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/video/background_segm.hpp>

using namespace cv;

class HumanDetector {
public:
  static std::vector<Rect> detect(Mat image_to_detect) {
    cv::HOGDescriptor hog;
    hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());

    std::vector<Rect> found_humans;
    hog.detectMultiScale(image_to_detect, found_humans, 0, cv::Size(8, 8),
                         cv::Size(16, 16), 1.05, 2);  // Default parameters.

    return found_humans;
  }
};

class HumanTracker {
public:
  static bool trackVideo(std::string video_file) {
    VideoCapture video(video_file);
    if(!video.isOpened()) {
      return false;
    }

    namedWindow("Display window",1);
    for (;;) {
        Mat frame;
        video >> frame;

        std::vector<Rect> detect_rects = HumanDetector::detect(frame);
        if (detect_rects.size() > 1) {
          printf("Start tracking...");
          // TODO(tullie): Tracking code here.
        }

        // Display.
        for (Rect detect_rect : detect_rects) {
          rectangle(frame, detect_rect, Scalar(0, 255, 0));
        }
        imshow("Display window", frame);

        if (waitKey(30) >= 0) {
          break;
        }
    }

    return true;
  }
};

#define RESIZE_FACTOR 2

RNG rng(12345);

class BackgroundMask {
public:
  static bool mask(std::string video_file) {
    Mat frame;
    Mat resizeF;
    Mat invF;
    Mat drawing;
    Mat fgMaskMOG;
    Mat fgMaskMOG2;

    Ptr<BackgroundSubtractor> pMOG = bgsegm::createBackgroundSubtractorMOG();
    VideoCapture video(video_file);

    if(!video.isOpened()) {
      return false;
    }

    namedWindow("Masked", 1);
    while(1) {
      if (!video.read(frame)) {
        break;
      }

      std::vector<Vec4i> hierarchy;
      std::vector<std::vector<Point> > contours;
      resize(frame, resizeF, Size(frame.size().width / RESIZE_FACTOR,
          frame.size().height / RESIZE_FACTOR));

      pMOG->apply(resizeF, fgMaskMOG);
      bitwise_not(fgMaskMOG, invF);

      Mat kernel = getStructuringElement(MORPH_ELLIPSE, Point(3, 3));
      morphologyEx(invF, drawing, MORPH_OPEN, kernel);

      // findContours(invF, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

      // drawing = Mat::zeros( invF.size(), CV_8UC3 );
      // for (int i = 0; i < contours.size(); ++i) {
      //   drawContours( drawing, contours, i, 255, 4);
      // }

      // imshow("Origin", resizeF);
      imshow("MOG", drawing);

      if (waitKey(1) >= 0) {
        break;
      }
    }

    return true;
  }
};

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Usage: %s <detection_video>", argv[0]);
    return -1;
  }

  BackgroundMask::mask(argv[1]);
  return 0;
}
