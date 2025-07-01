#include "reader.h"
#include <chrono>
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <thread>
#include <utility>

namespace read {

Reader::Reader(ts::TSQueue<cv::Mat> &output_queue_)
    : output_queue(output_queue_) {};

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

  int frame_count = 0;
  auto start = std::chrono::steady_clock::now();

  while (cap.isOpened()) {
    cv::Mat capture;
    cap >> capture;

    if (capture.empty())
      break;

    cv::Mat new_cap;
    cv::resize(capture, new_cap, cv::Size(320, 240));

    try {
      output_queue.push(new_cap);

      if (frame_count % 20 == 0) {
        std::cout << "Frame passed to queue " << new_cap.size << "\n";
      }
    } catch (...) {
      std::cerr << "Error: Exception caught.\n";
      break;
    }

    frame_count++;
    auto end = std::chrono::steady_clock::now();

    int elasped_secs =
        std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

    if (elasped_secs == 5) {
      float fps = frame_count / elasped_secs;
      std::cout << "Frame Reader FPS = " << fps << "\n";
      frame_count = 0;
      start = std::chrono::steady_clock::now();
    }
  }
  cap.release();
}

} // namespace read
