#pragma once

#include "customqueue.h"
#include "net.h"
#include "structs.h"
#include <opencv2/core.hpp>

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

class Emotion {
private:
  ncnn::Net emotion;

  cv::Mat track_frame;
  int image_w;
  int image_h;

  int num_threads;
  int img_width = 80;
  int img_height = 80;

  int predict(ncnn::Mat &img);

  int max(ncnn::Mat &probs);

public:
  Emotion(ts::TSQueue<std::unique_ptr<UltraStruct>> &input_queue_,
          const std::string &bin_path_, const std::string &param_path_,
          int num_threads_ = 4);

  Emotion(const Emotion &) = delete;            // Delete copy constructor
  Emotion &operator=(const Emotion &) = delete; // Delete copy assignment
  ~Emotion();

  ts::TSQueue<std::unique_ptr<UltraStruct>> &input_queue;

  const std::string &bin_path;
  const std::string &param_path;

  void infer();
};
