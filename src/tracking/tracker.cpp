#include "tracker.h"
#include <chrono>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <thread>
#include <utility>

Tracker::Tracker(ts::TSQueue<std::unique_ptr<UltraStruct>> &input_queue_,
                 float track_thresh_, int track_buffer_, float match_thresh_,
                 int frame_rate_)
    : input_queue(input_queue_) {
  track_thresh = track_thresh_;
  track_buffer = track_buffer_;
  match_thresh = match_thresh_;
  frame_rate = frame_rate_;
}

void Tracker::track() {
  cv::Mat ultra_frame;
  cv::Mat crop;
  int print_count = 0;

  cv::namedWindow("frame", cv::WINDOW_NORMAL);

  while (true) {
    if (!input_queue.empty()) {
      if (print_count % 20 == 0)
        std::cout << "Tracker now\n";

      cv::Mat crop;
      auto opt_ultra_ptr = input_queue.pop();

      if (opt_ultra_ptr.has_value()) {
        ultra_frame = opt_ultra_ptr.value()->frame;
      } else {
        std::cerr << "Error: opt_ultra_ptr is empty" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        continue;
      }

      /*
      for (int i = 0; i < 1; i++) {
        crop = x.crops[i];
      }
      */

      if (!ultra_frame.empty()) {
        /*
        int height = frame.rows;
        int height2 = crop.rows;

        if (print_count % 15 == 0)
          std::cout << "Track crop frame: " << height2 << "\n";
        */

        cv::imshow("frame", ultra_frame);
        cv::waitKey(1);

      } else {
        std::cout << "Tracker frame is empty.\n";
      }
    } else {
      std::this_thread::sleep_for(std::chrono::milliseconds(2));
      if (print_count % 40 == 0)
        std::cout << "Detection queue is empty\n";
    }
    print_count++;
  }
  cv::destroyAllWindows();
}
