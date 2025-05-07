#include "preprocessor.hpp"
#include "frame_reader.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <queue>

namespace process {

void Preprocess::display_frames() {
  auto &readerQueue = read::Reader::getInstance().getQueue();
  cv::namedWindow("Frame", cv::WINDOW_NORMAL);

  while (!readerQueue.empty()) {
    /**
if (readerQueue.empty())
  break;
    **/

    cv::Mat frame = readerQueue.front();
    std::cout << "Frame gotten from frame queue.\n";
    readerQueue.pop();

    if (!frame.empty()) {
      cv::imshow("Frame", frame);
      cv::resizeWindow("Frame", 640, 640);
    }

    if (cv::waitKey(113) == 'q') {
      break;
    }
  }

  cv::destroyAllWindows();
}
} // namespace process
