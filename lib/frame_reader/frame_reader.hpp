#pragma once

#include <opencv2/opencv.hpp>
#include <queue>
#include <string>
#include <variant>

namespace read {
class Reader {
private:
  Reader() = default;

  // Prevent copying
  Reader(const Reader &) = delete;
  Reader &operator=(const Reader &) = delete;

  std::variant<int, std::string> source;
  std::queue<cv::Mat> reader_queue;

public:
  // Singleton accessor
  static Reader &getInstance();

  // Configuration method
  void setSource(std::variant<int, std::string> s);

  std::queue<cv::Mat> &getQueue();
  void read_frames();
};

} // namespace read
