#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <variant>

#include "customqueue.h"
#include "reader.h"
#include "tracker.h"
#include "ultraface.h"

int main() {
  std::string input;
  std::string bin_path = "../../data/version-RFB/RFB-320.bin";
  std::string param_path = "../../data/version-RFB/RFB-320.param";
  std::cout << "Enter input source: ";
  std::cin >> input;

  std::variant<int, std::string> source;

  // Load bin and param files
  std::ifstream bin_file(bin_path, std::ios::binary);
  if (!bin_file) {
    std::cerr << "Can't open bin file\n";
    return 1;
  }

  std::ifstream param_file(param_path);
  if (!param_file) {
    std::cerr << "Can't open param file\n";
    return 1;
  }

  // Try to parse as int (webcam ID)
  try {
    source = std::stoi(input);
  } catch (...) {
    source = input;
  }

  ts::TSQueue<cv::Mat> reader_queue;
  ts::TSQueue<std::unique_ptr<UltraStruct>> detect_queue;

  read::Reader reader(reader_queue);
  reader.setSource(source);

  UltraFace ultraface(reader_queue, detect_queue, bin_path, param_path, 320,
                      240, 1,
                      0.7); // config model input

  Tracker tracks(detect_queue);

  std::thread t1([&]() { reader.read_frames(); });
  std::thread t2([&]() { ultraface.infer(); });
  std::thread t3([&]() { tracks.track(); });

  t1.join();
  t2.join();
  t3.join();
  return 0;
}
