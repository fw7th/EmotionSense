#include "emo.h"
#include "mat.h"

#include <cfloat>
#include <chrono>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <thread>
#include <utility>

Emotion::Emotion(ts::TSQueue<std::unique_ptr<UltraStruct>> &input_queue_,
                 const std::string &bin_path_, const std::string &param_path_,
                 int num_threads_)
    : input_queue(input_queue_), bin_path(bin_path_), param_path(param_path_)

{
  emotion.load_param(param_path.data());
  emotion.load_model(bin_path.data());
}

Emotion::~Emotion() { emotion.clear(); }

void Emotion::infer() {
  cv::Mat crop;
  int frame_count = 0;
  auto start = std::chrono::steady_clock::now();

  cv::namedWindow("frame", cv::WINDOW_NORMAL);

  while (true) {
    if (input_queue.empty()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(2));
      /*
      if (frame_count % 50 == 0) {
         std::cout << "Detection queue is empty\n";
      }
      */
    }
    /*
    if (frame_count % 40 == 0)
      std::cout << "Emotion now\n";
    */
    auto opt_track_ptr = std::move(input_queue.pop());

    if (!opt_track_ptr.has_value()) {
      std::cerr << "Error: opt_track_ptr is empty" << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(2));
      continue;
    }

    track_frame = opt_track_ptr.value()->frame;

    /*
    for (int i = 0; i < 1; i++) {
      crop = x.crops[i];
    }
    */

    if (track_frame.empty()) {
      // std::cout << "Emotion frame is empty.\n";
    }

    /*
    int height = frame.rows;
    int height2 = crop.rows;

    if (frame_count % 15 == 0)
      std::cout << "Track crop frame: " << height2 << "\n";
    */

    cv::imshow("frame", track_frame);
    cv::waitKey(1);

    frame_count++;
    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> elasped_seconds = end - start;
    auto duration_secs =
        std::chrono::duration_cast<std::chrono::seconds>(elasped_seconds);

    int secs = static_cast<int>(duration_secs.count());

    if (secs >= 1) {
      float fps = frame_count / 1.0;
      // std::cout << "[DEBUG] Emotion Reader FPS = " << fps << "\n";

      frame_count = 0;
      start = end;
    }
    frame_count++;
  }
  cv::destroyAllWindows();
}

int Emotion::predict(ncnn::Mat &img) {

  ncnn::Mat in;
  ncnn::resize_bilinear(img, in, img_width, img_height);
  ncnn::Mat ncnn_img = in;

  ncnn::Extractor ex = emotion.create_extractor();
  ex.set_num_threads(num_threads);
  ex.input("input", ncnn_img);

  ncnn::Mat out;
  ex.extract("probs", out);
  int max_index = max(out);
  return max_index;
}

int Emotion::max(ncnn::Mat &probs) {
  float max_val = -FLT_MAX;
  int max_index = -1;

  for (int i = 0; i < probs.w; ++i) {
    float val = probs[i];
    if (val > max_val) {
      max_val = val;
      max_index = i;
    }
  }
  return max_index;
}
