#include "frameReader.hpp"
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
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
  int i = 0;
  while (cap.isOpened()) {
    cv::Mat capture;
    cap >> capture;

    if (capture.empty())
      break;

    cv::Mat new_cap;
    cv::resize(capture, new_cap, cv::Size(320, 240));

    i++;
    try {
      reader_queue.push(std::move(new_cap));
      if (i % 8 == 0) {
        std::cout << "Frame passed to queue " << new_cap.size << "\n";
      }
    } catch (...) {
      std::cerr << "Error: Exception caught.\n";
      break;
    }
  }
  cap.release();
}

} // namespace read
