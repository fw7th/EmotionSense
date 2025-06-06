#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <variant>

#include "UltraFace.hpp"
#include "frameReader.hpp"
#include "tracker.h"

int main() {
  std::string input;
  std::string bin_path = "../data/version-RFB/RFB-320.bin";
  std::string param_path = "../data/version-RFB/RFB-320.param";
  std::cout << "Enter input source: ";
  std::cin >> input;

  std::variant<int, std::string> source;

  // Load bin and weights
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

  read::Reader shared_obj1;
  shared_obj1.setSource(source);

  UltraFace ultraface(shared_obj1, bin_path, param_path, 320, 240, 1,
                      0.7); // config model input

  Tracker tracks(&ultraface);

  std::thread t1([&]() { shared_obj1.read_frames(); });
  std::thread t2([&]() { ultraface.infer(); });
  std::thread t3([&]() { tracks.track(); });

  t1.join();
  t2.join();
  t3.join();
  return 0;
}
