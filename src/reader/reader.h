#pragma once

#include "customqueue.h"
#include <opencv2/core.hpp>
#include <string>
#include <variant>

namespace read {
class Reader {
private:
  std::variant<int, std::string> source;

public:
  Reader(ts::TSQueue<cv::Mat> &output_queue_);
  ts::TSQueue<cv::Mat> &output_queue;

  // Configuration method
  void setSource(std::variant<int, std::string> s);
  void read_frames();
};

} // namespace read
