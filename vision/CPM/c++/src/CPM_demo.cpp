#include <caffe/caffe.hpp>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace caffe;
using namespace cv;

void WrapInputLayer(std::vector<Mat>* input_channels, const Net<float>& net) {
  Blob<float>* input_layer = net.input_blobs()[0];

  int width = input_layer->width();
  int height = input_layer->height();
  float* input_data = input_layer->mutable_cpu_data();
  std::cout << input_layer->channels() << std::endl;
  for (int i = 0; i < input_layer->channels(); ++i) {
    Mat channel(height, width, CV_32FC1, input_data);
    input_channels->push_back(channel);
    input_data += width * height;
  }
}

void Preprocess(const Mat& img, std::vector<Mat>* input_channels,
                int num_channels, const Size& input_geometry) {

  Mat sample;
  if (img.channels() == 3) {
    cvtColor(img, sample, COLOR_RGB2BGRA);
  } else if (img.channels() == 4) {
    cvtColor(img, sample, COLOR_RGBA2BGRA);
    sample = img;
  } else {
    printf("Image not supported");
    return;
  }

  Mat sample_resized;
  if (sample.size() != input_geometry)
    resize(sample, sample_resized, input_geometry);
  else
    sample_resized = sample;

  for (int i = 0; i < sample_resized.size().width; ++i) {
    for (int j = 0; j < sample_resized.size().height; ++j) {
      sample_resized.at<Vec4f>(i, j)[0] = sample_resized.at<Vec4f>(i, j)[0] / 255 - 0.5;
      sample_resized.at<Vec4f>(i, j)[1] = sample_resized.at<Vec4f>(i, j)[1] / 255 - 0.5;
      sample_resized.at<Vec4f>(i, j)[2] = sample_resized.at<Vec4f>(i, j)[2] / 255 - 0.5;
      sample_resized.at<Vec4f>(i, j)[3] = 1;
    }
  }

  split(sample_resized, *input_channels);
}

int main(int argc, char** argv) {
  if (argc != 4) {
    printf("Usage %s <model_file> <trained_file> <test_image>", argv[0]);
    return -1;
  }

  std::string model_file = argv[1];
  std::string trained_file = argv[2];
  std::string test_image_file = argv[3];
  Mat test_image = imread(test_image_file, IMREAD_UNCHANGED);

  // Initialize Caffe.
  Caffe::set_mode(Caffe::CPU);
  Net<float> net(model_file, TEST);
  net.CopyTrainedLayersFrom(trained_file);
  Blob<float>* input_layer = net.input_blobs()[0];
  int num_channels = input_layer->channels();
  Size input_geometry = Size(input_layer->width(), input_layer->height());
  input_layer->Reshape(1, num_channels, input_geometry.height,
                       input_geometry.width);
  net.Reshape();

  std::vector<cv::Mat> input_channels;
  WrapInputLayer(&input_channels, net);

  Preprocess(test_image, &input_channels, num_channels, input_geometry);

  net.Forward();

  Blob<float>* output_layer = net.output_blobs()[0];
  std::cout << output_layer->shape_string() << std::endl;

  float* channel = output_layer->mutable_cpu_data();
  for (int i = 0; i < output_layer->channels(); ++i) {
    Mat class_heatmap(output_layer->height(), output_layer->width(), CV_32F,
                      channel);
    channel += output_layer->width() * output_layer->height();

    double min, max;
    cv::minMaxLoc(class_heatmap, &min, &max);
    for (int i = 0; i < class_heatmap.size().width; ++i) {
      for (int j = 0; j < class_heatmap.size().height; ++j) {
        class_heatmap.at<float>(i, j) = (class_heatmap.at<float>(i, j) - min) / (max - min);
      }
    }
    std::cout << class_heatmap << std::endl;

    // circle(class_heatmap, max_loc, 1, Scalar(255, 255, 255), 3);
    // circle(class_heatmap, min_loc, 1, Scalar(255, 255, 255), 3);

    namedWindow("Display View", CV_WINDOW_AUTOSIZE);
    imshow("Display View", class_heatmap);
    waitKey(0);
  }
}
