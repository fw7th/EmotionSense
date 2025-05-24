#pragma once

#include <opencv2/opencv.hpp>
#include <queue>
#include <string>
#include <variant>

namespace read {
class Reader {
private:
  std::variant<int, std::string> source;
  std::queue<cv::Mat> reader_queue;

public:
  // Configuration method void setSource(std::variant<int, std::string> s);

  void read_frames();
};

} // namespace read
