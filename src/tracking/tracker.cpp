#include "tracker.h"
// #include "BYTETracker.h"
#include "UltraFace.hpp"
#include <iostream>
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

  while (!ultraface_queue_ref.empty()) {
    std::vector<UltraStruct> ultra_struct = ultraface_queue_ref.front();
    cv::Mat crop;
    ultraface_queue_ref.pop();

    for (int i = 0; i < ultra_struct.size(); i++) {
      auto ultra = ultra_struct[i];
      frame = ultra.frame;
    }

    cv::imshow("frame", frame);
  }
}
