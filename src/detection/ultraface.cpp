//
//  UltraFace.cpp
//  UltraFaceTest
//
//  Created by vealocia on 2019/10/17.
//  Copyright © 2019 vealocia. All rights reserved.
//

#define clip(x, y) (x < 0 ? 0 : (x > y ? y : x))

#include "ultraface.h"
#include "funcs.h"
#include "mat.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <thread>
#include <utility>

UltraFace::UltraFace(ts::TSQueue<cv::Mat> &input_queue_,
                     ts::TSQueue<std::unique_ptr<UltraStruct>> &output_queue_,
                     const std::string &bin_path_,
                     const std::string &param_path_, int input_width,
                     int input_length, int num_thread_, float score_threshold_,
                     float iou_threshold_, int topk_)
    : input_queue(input_queue_), output_queue(output_queue_),
      bin_path(bin_path_), param_path(param_path_)

{
  num_thread = num_thread_;
  topk = topk_;
  score_threshold = score_threshold_;
  iou_threshold = iou_threshold_;
  in_w = input_width;
  in_h = input_length;
  w_h_list = {in_w, in_h};

  for (auto size : w_h_list) {
    std::vector<float> fm_item;
    for (float stride : strides) {
      fm_item.push_back(ceil(size / stride));
    }
    featuremap_size.push_back(fm_item);
  }

  for (auto size : w_h_list) {
    shrinkage_size.push_back(strides);
  }

  /* generate prior anchors */
  for (int index = 0; index < num_featuremap; index++) {
    float scale_w = in_w / shrinkage_size[0][index];
    float scale_h = in_h / shrinkage_size[1][index];
    for (int j = 0; j < featuremap_size[1][index]; j++) {
      for (int i = 0; i < featuremap_size[0][index]; i++) {
        float x_center = (i + 0.5) / scale_w;
        float y_center = (j + 0.5) / scale_h;

        for (float k : min_boxes[index]) {
          float w = k / in_w;
          float h = k / in_h;
          priors.push_back(
              {clip(x_center, 1), clip(y_center, 1), clip(w, 1), clip(h, 1)});
        }
      }
    }
  }
  num_anchors = priors.size();
  /* generate prior anchors finished */

  ultraface.load_param(param_path.data());
  ultraface.load_model(bin_path.data());
}

UltraFace::~UltraFace() { ultraface.clear(); }

void UltraFace::infer() {
  int frame_count = 0;
  auto fps_timer_start = std::chrono::steady_clock::now();

  // Pre-allocate reusable containers
  std::vector<FaceInfo> face_info;
  face_info.reserve(10); // Reserve space for typical number of faces

  while (true) {
    auto loop_start = std::chrono::steady_clock::now();

    // Check queue and get frame
    if (input_queue.empty()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(2));
      continue;
    }

    auto opt_frame = std::move(input_queue.pop());
    if (!opt_frame.has_value()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(2));
      continue;
    }

    cv::Mat &frame = opt_frame.value();

    // Validate frame
    if (frame.empty() || frame.channels() != 3) {
      if (frame.channels() != 3) {
        std::cerr << "Unexpected number of channels: " << frame.channels()
                  << "\n";
      }
      continue;
    }

    // Time frame cloning
    auto copy_start = std::chrono::steady_clock::now();
    cv::Mat frame_copy = frame.clone();
    auto copy_end = std::chrono::steady_clock::now();
    auto copy_time = std::chrono::duration_cast<std::chrono::duration<double>>(
        copy_end - copy_start);

    // Convert to ncnn format
    ncnn::Mat inmat = ncnn::Mat::from_pixels(
        frame.data, ncnn::Mat::PIXEL_BGR2RGB, frame.cols, frame.rows);

    // Clear and prepare containers
    face_info.clear();
    auto obj_ptr = std::make_unique<UltraStruct>();

    // Time face detection
    auto detect_start = std::chrono::steady_clock::now();
    detect(inmat, face_info);
    auto detect_end = std::chrono::steady_clock::now();
    auto detect_time =
        std::chrono::duration_cast<std::chrono::duration<double>>(detect_end -
                                                                  detect_start);

    if (face_info.empty()) {
      continue;
    }

    // Pre-allocate crops vector
    obj_ptr->crops.reserve(face_info.size());

    // Time ROI processing (all faces together)
    auto roi_start = std::chrono::steady_clock::now();
    for (const auto &face :
         face_info) { // Use const reference, range-based loop
      cv::Point pt1(static_cast<int>(face.x1), static_cast<int>(face.y1));
      cv::Point pt2(static_cast<int>(face.x2), static_cast<int>(face.y2));

      cv::Mat crop = roiCrop(face.x1, face.y1, face.x2, face.y2, frame_copy);
      obj_ptr->crops.emplace_back(std::move(crop)); // Move crop to avoid copy
      cv::rectangle(frame, pt1, pt2, cv::Scalar(0, 255, 0), 1);
    }
    auto roi_end = std::chrono::steady_clock::now();
    auto roi_time = std::chrono::duration_cast<std::chrono::duration<double>>(
        roi_end - roi_start);

    // Move frame to output object
    obj_ptr->frame = std::move(frame);

    // Push to output queue
    output_queue.push(std::move(obj_ptr));

    // Calculate total loop time
    auto loop_end = std::chrono::steady_clock::now();
    auto loop_time = std::chrono::duration_cast<std::chrono::duration<double>>(
        loop_end - loop_start);

    // Print detailed timing every 10 frames
    if (frame_count % 10 == 0) {
      std::cout << "=== FRAME " << frame_count << " TIMING ===\n";
      std::cout << "[TIMING] Frame Copy:     " << std::fixed
                << std::setprecision(2) << copy_time.count() * 1000 << " ms\n";
      std::cout << "[TIMING] Face Detection: " << std::fixed
                << std::setprecision(2) << detect_time.count() * 1000
                << " ms\n";
      std::cout << "[TIMING] ROI Processing: " << std::fixed
                << std::setprecision(2) << roi_time.count() * 1000 << " ms\n";
      std::cout << "[TIMING] Total Loop:     " << std::fixed
                << std::setprecision(2) << loop_time.count() * 1000 << " ms\n";
      std::cout << "[TIMING] Faces Detected: " << face_info.size() << "\n";
      if (!face_info.empty()) {
        std::cout << "[TIMING] Avg ROI/Face:   " << std::fixed
                  << std::setprecision(2)
                  << (roi_time.count() * 1000) / face_info.size() << " ms\n";
      }
      std::cout << "================================\n";
    }

    // FPS calculation
    frame_count++;
    auto fps_timer_end = std::chrono::steady_clock::now();
    auto elapsed_seconds =
        std::chrono::duration_cast<std::chrono::duration<double>>(
            fps_timer_end - fps_timer_start);

    if (elapsed_seconds.count() >= 1.0) {
      double fps = frame_count / elapsed_seconds.count();
      std::cout << "[DEBUG] Face Detector FPS = " << std::fixed
                << std::setprecision(1) << fps << "\n";
      frame_count = 0;
      fps_timer_start = fps_timer_end;
    }
  }
}

cv::Mat UltraFace::roiCrop(float x1, float y1, float x2, float y2,
                           cv::Mat &frame) {
  float width = (x2 - x1) + 35;
  float height = (y2 - y1) + 35;
  cv::Rect roi(x1 - 20, y1 - 20, width, height);
  cv::Mat cropped = frame(roi);

  /*
  std::cout << "Cropped size: " << cropped.cols << "x" << cropped.rows
            << std::endl;
  std::cout << "Cropped empty? " << cropped.empty() << std::endl;
  */

  if (cropped.empty()) {

    std::cout << "Cropped image is empty!" << std::endl;
  }
  cv::Mat resize_cropped;
  cv::Size newSize(48, 48);

  cv::resize(cropped, resize_cropped, newSize, 0, 0, cv::INTER_LINEAR);
  return resize_cropped;
}

int UltraFace::detect(ncnn::Mat &img, std::vector<FaceInfo> &face_list) {
  if (img.empty()) {
    std::cout << "image is empty ,please check!" << std::endl;
    return -1;
  }

  image_h = img.h;
  image_w = img.w;

  ncnn::Mat in;
  ncnn::resize_bilinear(img, in, in_w, in_h);
  ncnn::Mat ncnn_img = in;
  ncnn_img.substract_mean_normalize(mean_vals, norm_vals);

  std::vector<FaceInfo> bbox_collection;
  std::vector<FaceInfo> valid_input;

  ncnn::Extractor ex = ultraface.create_extractor();
  ex.set_num_threads(num_thread);
  ex.input("input", ncnn_img);

  ncnn::Mat scores;
  ncnn::Mat boxes;
  ex.extract("scores", scores);
  ex.extract("boxes", boxes);
  generateBBox(bbox_collection, scores, boxes, score_threshold, num_anchors);
  nms(bbox_collection, face_list);
  return 0;
}

void UltraFace::generateBBox(std::vector<FaceInfo> &bbox_collection,
                             ncnn::Mat scores, ncnn::Mat boxes,
                             float score_threshold, int num_anchors) {
  for (int i = 0; i < num_anchors; i++) {
    if (scores.channel(0)[i * 2 + 1] > score_threshold) {
      FaceInfo rects;
      float x_center =
          boxes.channel(0)[i * 4] * center_variance * priors[i][2] +
          priors[i][0];
      float y_center =
          boxes.channel(0)[i * 4 + 1] * center_variance * priors[i][3] +
          priors[i][1];
      float w = exp(boxes.channel(0)[i * 4 + 2] * size_variance) * priors[i][2];
      float h = exp(boxes.channel(0)[i * 4 + 3] * size_variance) * priors[i][3];

      rects.x1 = clip(x_center - w / 2.0, 1) * image_w;
      rects.y1 = clip(y_center - h / 2.0, 1) * image_h;
      rects.x2 = clip(x_center + w / 2.0, 1) * image_w;
      rects.y2 = clip(y_center + h / 2.0, 1) * image_h;
      rects.score = clip(scores.channel(0)[i * 2 + 1], 1);
      bbox_collection.push_back(rects);
    }
  }
}

void UltraFace::nms(std::vector<FaceInfo> &input, std::vector<FaceInfo> &output,
                    int type) {
  std::sort(
      input.begin(), input.end(),
      [](const FaceInfo &a, const FaceInfo &b) { return a.score > b.score; });

  int box_num = input.size();

  std::vector<int> merged(box_num, 0);

  for (int i = 0; i < box_num; i++) {
    if (merged[i])
      continue;
    std::vector<FaceInfo> buf;

    buf.push_back(input[i]);
    merged[i] = 1;

    float h0 = input[i].y2 - input[i].y1 + 1;
    float w0 = input[i].x2 - input[i].x1 + 1;

    float area0 = h0 * w0;

    for (int j = i + 1; j < box_num; j++) {
      if (merged[j])
        continue;

      float inner_x0 = input[i].x1 > input[j].x1 ? input[i].x1 : input[j].x1;
      float inner_y0 = input[i].y1 > input[j].y1 ? input[i].y1 : input[j].y1;

      float inner_x1 = input[i].x2 < input[j].x2 ? input[i].x2 : input[j].x2;
      float inner_y1 = input[i].y2 < input[j].y2 ? input[i].y2 : input[j].y2;

      float inner_h = inner_y1 - inner_y0 + 1;
      float inner_w = inner_x1 - inner_x0 + 1;

      if (inner_h <= 0 || inner_w <= 0)
        continue;

      float inner_area = inner_h * inner_w;

      float h1 = input[j].y2 - input[j].y1 + 1;
      float w1 = input[j].x2 - input[j].x1 + 1;

      float area1 = h1 * w1;

      float score;

      score = inner_area / (area0 + area1 - inner_area);

      if (score > iou_threshold) {
        merged[j] = 1;
        buf.push_back(input[j]);
      }
    }
    switch (type) {
    case hard_nms: {
      output.push_back(buf[0]);
      break;
    }
    case blending_nms: {
      float total = 0;
      for (int i = 0; i < buf.size(); i++) {
        total += exp(buf[i].score);
      }
      FaceInfo rects;
      memset(&rects, 0, sizeof(rects));
      for (int i = 0; i < buf.size(); i++) {
        float rate = exp(buf[i].score) / total;
        rects.x1 += buf[i].x1 * rate;
        rects.y1 += buf[i].y1 * rate;
        rects.x2 += buf[i].x2 * rate;
        rects.y2 += buf[i].y2 * rate;
        rects.score += buf[i].score * rate;
      }
      output.push_back(rects);
      break;
    }
    default: {
      printf("wrong type of nms.");
      exit(-1);
    }
    }
  }
}
