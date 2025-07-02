#include "tracker.h"
#include <chrono>
#include <iostream>
#include <thread>
#include <utility>

Tracker::Tracker(ts::TSQueue<std::unique_ptr<UltraStruct>> &input_queue_,
                 ts::TSQueue<std::unique_ptr<UltraStruct>> &output_queue_,
                 float track_thresh_, int track_buffer_, float match_thresh_,
                 int frame_rate_)
    : input_queue(input_queue_), output_queue(output_queue_) {
  track_thresh = track_thresh_;
  track_buffer = track_buffer_;
  match_thresh = match_thresh_;
  frame_rate = frame_rate_;
}

void Tracker::track() {
  while (true) {
    if (input_queue.empty()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(2));
      /*
      if (frame_count % 50 == 0) {
         std::cout << "Detection queue is empty\n";
      }
      */
    }
    auto opt_ultra_ptr = std::move(input_queue.pop());

    if (!opt_ultra_ptr.has_value()) {
      // std::cerr << "Error: opt_ultra_ptr is empty" << std::endl;

      std::this_thread::sleep_for(std::chrono::milliseconds(2));
      continue;
    }

    auto &ultra_ptr = opt_ultra_ptr.value();

    // Do Tracker Work

    output_queue.push(ultra_ptr);
  }
}
