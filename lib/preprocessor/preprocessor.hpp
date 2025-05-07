#pragma once

#include <opencv2/opencv.hpp>
#include <queue>

namespace process {
class Preprocess {
private:
  std::queue<cv::Mat> process_queue;

public:
  Preprocess() = default;

  void display_frames();
};
} // namespace process
