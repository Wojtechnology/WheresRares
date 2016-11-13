#include <iostream>
#include <list>
#include <map>
#include <vector>
#include "opencv/cv.h"

#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "HumanTracking.pch"

using namespace std;
using namespace cv;

FuncPtr Debug;

void SetDebugFunction( FuncPtr fp ) {
  Debug = fp;
}

class HumanDetector {
 public:
  HumanDetector(int view_width, int view_height)
      : view_width_(view_width), view_height_(view_height) {
    hog_.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
  }

  vector<Rect> detect(Mat image_to_detect) {
    vector<Rect> detected_humans;
    hog_.detectMultiScale(image_to_detect, detected_humans, 0, cv::Size(8, 8),
                          cv::Size(32, 32), 1.05, 2);  // Default parameters.

    // Filter redundant rectangles.
    vector<Rect> filtered_detected_humans;
    for (int i = 0; i < detected_humans.size(); i++) {
      Rect r = detected_humans[i];
      int j;
      for (j = 0; j < detected_humans.size(); j++) {
        if (j != i && (r & detected_humans[j]) == r) {
          break;
        }
      }

      if (j == detected_humans.size()) {
        filtered_detected_humans.push_back(r);
      }
    }

    // Bound rectangles to screen.
    for (int i = 0; i < filtered_detected_humans.size(); i++) {
      if (filtered_detected_humans[i].x < 0) {
        filtered_detected_humans[i].x = 0;
      }
      if (filtered_detected_humans[i].y < 0) {
        filtered_detected_humans[i].y = 0;
      }
      if (filtered_detected_humans[i].x + filtered_detected_humans[i].width >
          view_width_) {
        filtered_detected_humans[i].width =
            view_width_ - filtered_detected_humans[i].x;
      }
      if (filtered_detected_humans[i].y + filtered_detected_humans[i].height >
          view_height_) {
        filtered_detected_humans[i].height =
            view_height_ - filtered_detected_humans[i].y;
      }
    }

    return filtered_detected_humans;
  }

 private:
  HOGDescriptor hog_;
  int view_width_;
  int view_height_;
};


const int kFrameQueueSize = 50;
const float kThresholdPercentage = 0.1;
const bool kDrawBoxesGrouped = true;

HumanDetector human_detector = HumanDetector(0,0);
vector<Point2f> features;
int features_number = 0;
vector<int> setNumbers;

vector<Point2f> find_features(Mat image, vector<Rect> detected_bounding,
                              int& features_number) {
  vector<Point2f> corners;
  list<CvPoint2D32f> features;
  list<int> set_number;

  int i;
  int all_features_number = 0;
  for (i = 0; i < detected_bounding.size(); i++) {
    Rect human_detector = detected_bounding[i];
    Mat cropped_rectangle;
    image(human_detector).copyTo(cropped_rectangle);
    goodFeaturesToTrack(cropped_rectangle, corners, 300, 0.01, 10);
    for (int k = 0; k < corners.size(); k++) {
      corners[k].x = human_detector.x + corners[k].x;
      corners[k].y = human_detector.y + corners[k].y;
      features.push_back(corners[k]);
      set_number.push_back(i);
    }
    all_features_number += corners.size();
  }

  vector<Point2f> features_res;
  setNumbers = vector<int>(all_features_number);
  int iter = 0;
  while (features.size() > 0) {
    CvPoint2D32f F_point = features.front();
    features_res.emplace_back(F_point.x, F_point.y);
    features.pop_front();

    int SN = set_number.front();
    setNumbers[iter] = SN;
    set_number.pop_front();
  }
  features_number = all_features_number;
  return features_res;
}

void InitCameraPlugin(int height, int width) {
    human_detector = HumanDetector(width, height);
}

int* GetBoundingBox(int map[]) {
    int* boundingBox = new int[4];
    boundingBox[0] = 100;
    boundingBox[1] = 100;
    boundingBox[2] = 500;
    boundingBox[3] = 1000;
    return boundingBox;
}

int* GetQuickerBoundingBox(unsigned char* map, int height, int width) {
//    Mat current_frame_copy;
//    Mat current_frame = Mat(height,width,CV_8UC4,map);
//    // detection step
//    current_frame.copyTo(current_frame_copy);
//    cvtColor(current_frame, current_frame, CV_BGR2GRAY);
//    vector<Rect> found_filtered;
//    found_filtered = human_detector.detect(current_frame);
    Debug("poop");
    return nullptr;
}

bool DetectPerson(unsigned char* map, int height, int width) {
    Mat current_frame_copy;
    Mat current_frame = Mat(height,width,CV_8UC4,map);
    //HumanDetector human_detector(width, height);
    // detection step
    current_frame.copyTo(current_frame_copy);
    cvtColor(current_frame, current_frame, CV_BGR2GRAY);
    vector<Rect> found_filtered;
    found_filtered = human_detector.detect(current_frame);
    if (found_filtered.size() > 0) {
        features =
        find_features(current_frame, found_filtered, features_number);
        return true;
    }
    return false;
    //imshow("Display View", current_frame_copy);
    //waitKey(5);
}

int main(int argc, const char* argv[]) {
  if (argc > 2) {
    cout << "Usage: " << argv[0] << " [video_path]" << endl;
  }

  int screen_width;
  int screen_height;
  VideoCapture video_capture;
  if (argc == 2) {
    VideoCapture file_capture(argv[1]);
    screen_width = file_capture.get(CV_CAP_PROP_FRAME_WIDTH);
    screen_height = file_capture.get(CV_CAP_PROP_FRAME_HEIGHT);
    video_capture = file_capture;
  } else {
    // Capture from webcam.
    VideoCapture webcam_capture(0);
    screen_width = 640;
    screen_height = 480;
    webcam_capture.set(CV_CAP_PROP_FRAME_WIDTH,
                       screen_width);  // 1024,320,640,160
    webcam_capture.set(CV_CAP_PROP_FRAME_HEIGHT,
                       screen_height);  // 768,240,480,120
    video_capture = webcam_capture;
  }

  namedWindow("Display View", CV_WINDOW_AUTOSIZE);
  Mat current_frame;
  Mat next_frame;
  Mat current_frame_copy;
  HumanDetector human_detector(screen_width, screen_height);
  while (1) {
    while (1) {
      cout << "Start detection loop...\n";
      video_capture >> current_frame;
      current_frame.copyTo(current_frame_copy);
      cvtColor(current_frame, current_frame, CV_BGR2GRAY);
      vector<Rect> found_filtered;
      found_filtered = human_detector.detect(current_frame);
      if (found_filtered.size() > 0) {
        features =
            find_features(current_frame, found_filtered, features_number);
        break;
      }

      imshow("Display View", current_frame_copy);
      waitKey(5);
    }

    int threshold = features_number * kThresholdPercentage;
    vector<int> feature_counts;

    while (1) {
      cout << "Start Track loop..." << endl;
      feature_counts.push_back(features_number);

      // Break when features don't change e.g. if tree is detected.
      if (feature_counts.size() >= kFrameQueueSize &&
          abs(feature_counts[feature_counts.size() - 1] -
              feature_counts[feature_counts.size() - 2]) < 2) {
        cout << "Feature count has not changed for a while." << endl;
        break;
      }

      // Break if not enough features found.
      if (features_number < threshold) {
        cout << "Features less than threshold." << endl;
        break;
      }

      current_frame_copy.setTo(Scalar(0, 0, 0));
      video_capture >> next_frame;
      next_frame.copyTo(current_frame_copy);
      cvtColor(next_frame, next_frame, CV_BGR2GRAY);

      // List of lost features.
      vector<Point2f> tracked_features;
      vector<uchar> status;
      vector<float> feature_errors;
      calcOpticalFlowPyrLK(current_frame, next_frame, features,
                           tracked_features, status, feature_errors);

      // Update tracking info.
      int new_feature_number = 0;
      vector<int> temp_set_number(features_number);
      int j = 0;
      for (int i = 0; i < features_number; i++) {
        if (status[i] == 1) {
          new_feature_number++;
          temp_set_number[j] = setNumbers[i];
          j++;
        }
      }
      setNumbers = temp_set_number;
      features_number = new_feature_number;
      features = tracked_features;
      next_frame.copyTo(current_frame);

      // Display bounding boxes.
      if (kDrawBoxesGrouped) {
        Rect human_rect = boundingRect(features);
        const int kHumanWidth = 100;
        const int kHumanHeight = 200;

        // Construct box at box center.
        //Point box_center(human_rect.size().width / 2 + human_rect.x,
        //                 human_rect.size().height / 2 + human_rect.y);
        // Rect new_human_rect =
        //    Rect(box_center.x - kHumanWidth / 2,
        //         box_center.y - kHumanHeight / 2, kHumanWidth, kHumanHeight);

        rectangle(current_frame_copy, human_rect, Scalar(0, 255, 0));

        // Filter box based on aspect ratio.
        // double aspect_ratio = human_rect.width / (double)human_rect.height;
        // if (aspect_ratio > 0.5 && aspect_ratio < 1.5) {
        //   rectangle(current_frame_copy, human_rect, Scalar(0, 255, 0));
        // }

      } else {
        map<int, vector<Point2f>> split_points;
        for (int i = 0; i < features_number; ++i) {
          split_points[setNumbers[i]].push_back(features[i]);
        }

        for (const auto& it : split_points) {
          Rect human_rect = boundingRect(it.second);
          rectangle(current_frame_copy, human_rect, Scalar(0, 255, 0));
        }
      }

      // Display points.
      for (int i = 0; i < features_number; i++) {
        circle(current_frame_copy, features[i], 1,
               Scalar(0, 255 * setNumbers[i], 255 * setNumbers[i]), 3);
      }

      imshow("Display View", current_frame_copy);
      waitKey(10);
    }
  }

  return 0;
}
