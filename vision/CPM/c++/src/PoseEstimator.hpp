#pragma once

#include <caffe/caffe.hpp>
#include <opencv2/core/core.hpp>

class PoseEstimator {
 public:
  PoseEstimator();

  // Human rect must be a square.
  // Returns limbs in order:
  // {'head', 'neck', 'Rsho', 'Relb', 'Rwri', 'Lsho', 'Lelb', 'Lwri', 'Rhip',
  // 'Rkne', 'Rank', 'Lhip', 'Lkne', 'Lank', 'bkg'}
  std::vector<cv::Point> detectLimbs(cv::Mat human_rect);

 private:
  const std::string kModelFile =
      "../../model/_trained_MPI/pose_deploy_centerMap.prototxt";
  const std::string kTrainedFile =
      "../../model/_trained_MPI/pose_iter_630000.caffemodel";

  std::unique_ptr<caffe::Net<float>> net_;
  cv::Size input_geometry_;
  int num_channels_;
};
