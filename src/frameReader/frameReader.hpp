#pragma once

#ifndef TRACKER_H
#define TRACKER_H

#include <opencv2/opencv.hpp>
#include <queue>
#include <string>
#include <variant>

namespace read {
class Reader {
private:
  std::variant<int, std::string> source;

public:
  // Configuration method
  void setSource(std::variant<int, std::string> s);
  std::queue<cv::Mat> reader_queue;

  void read_frames();
};

} // namespace read

#endif
