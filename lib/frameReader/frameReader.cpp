#include "frameReader.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <utility>

namespace read {

void Reader::setSource(std::variant<int, std::string> s) {
  source = std::move(s);
}

void Reader::read_frames() {

  // Check if source is set
  if (std::holds_alternative<std::string>(source) &&
      std::get<std::string>(source).empty()) {
    std::cerr << "Source not set, cannot read frames\n ";
    return;
  }

  cv::VideoCapture cap;

  // resolve variant
  std::visit([&cap](auto &&value) { cap.open(value); }, source);

  if (!cap.isOpened()) {
    std::cerr << "Source could not be read. \n";
    return;
  }

  while (true) {
    cv::Mat capture;
    int width = 320;
    int height = 240;

    cap.read(&capture);
    if (&capture.empty())
      break;

    try {
      &reader_queue.push(&capture);
    } catch (...) {
      std::cerr << "Error: Exception caught.\n";
      break;
    }
  }

  cap.release();
}
} // namespace read
