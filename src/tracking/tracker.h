#pragma once

#include "customqueue.h"
#include "structs.h"
#include <memory>
#include <opencv2/core.hpp>

class Tracker {
public:
  ts::TSQueue<std::unique_ptr<UltraStruct>> &input_queue;

  Tracker(ts::TSQueue<std::unique_ptr<UltraStruct>> &input_queue_,
          float track_thresh_ = 0.23f, int track_buffer_ = 30,
          float match_thresh_ = 0.8, int frame_rate_ = 30);

  Tracker(const Tracker &) = delete;            // Delete copy constructor
  Tracker &operator=(const Tracker &) = delete; // Delete copy assignment
  void track();

  std::queue<cv::Mat> tracker_queue;

  typedef struct TrackedFaces {
    int id;
    float pt1;
    cv::Mat crop;

  } TrackedFaces;

private:
  float track_thresh;
  int track_buffer;
  float match_thresh;
  int frame_rate;
};
