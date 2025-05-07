#include <iostream>
#include <queue>
#include <string>
#include <variant>

#include "frame_reader.hpp"
#include "preprocessor.hpp"

int main() {
  std::string input;
  std::cout << "Enter input source: ";
  std::cin >> input;

  std::variant<int, std::string> source;

  // Try to parse as int (webcam ID)
  try {
    source = std::stoi(input);
  } catch (...) {
    source = input;
  }

  read::Reader &r = read::Reader::getInstance();
  process::Preprocess p;
  r.setSource(source);
  r.read_frames();
  p.display_frames();
  return 0;
}
