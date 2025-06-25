#include "tracker.h"
#include "UltraFace.hpp"
#include <chrono>
#include <iostream>
#include <memory>
#include <opencv2/highgui.hpp>
#include <thread>
#include <utility>

Tracker::Tracker(UltraFace *instance, float track_thresh_, int track_buffer_,
                 float match_thresh_, int frame_rate_)
    : interface(instance) {
  track_thresh = track_thresh_;
  track_buffer = track_buffer_;
  match_thresh = match_thresh_;
  frame_rate = frame_rate_;
}

/*
BYTETracker::tracker(float track_thresh, int track_buffer, float match_thresh,
                     int frame_rate);
*/

void Tracker::track() {

  auto &ultraface_queue_ref = interface->ultraface_queue;
  cv::Mat frame;
  cv::Mat crop;
  int print_count = 0;

  cv::namedWindow("frame", cv::WINDOW_NORMAL);

  while (true) {
    if (!ultraface_queue_ref.empty()) {
      auto ultra_ptr = *(ultraface_queue_ref.front());
      if (print_count % 10 == 0)
        std::cout << "Tracker now\n";

      cv::Mat crop;
      ultraface_queue_ref.pop();

      for (auto &x : ultra_ptr) {
        frame = x.frame;

        for (int i = 0; i < 1; i++) {
          crop = x.crops[i];
        }
      }

      if (!frame.empty()) {
        int height = frame.rows;
        int height2 = crop.rows;

        if (print_count % 10 == 0)
          std::cout << "Track crop frame: " << height2 << "\n";

        cv::imshow("frame", crop);
        cv::waitKey(10);

      } else {
        std::cout << "Tracker frame is empty.\n";
      }
    } else {
      std::this_thread::sleep_for(std::chrono::milliseconds(2));
      if (print_count % 15 == 0)
        std::cout << "Detection queue is empty\n";
    }
    print_count++;
  }
  cv::destroyAllWindows();
}
