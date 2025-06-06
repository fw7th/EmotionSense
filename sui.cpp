
#include <iostream>
#include <opencv2/opencv.hpp>

int main() {
  /*
cv::Mat image = cv::imread("/home/fw7th/Pictures/car.jpeg");
if (image.empty()) {
  std::cerr << "Error: Image not loaded. Check the path.\n";
  return -1;
}

// Check image size
std::cout << "Image size: " << image.cols << "x" << image.rows << std::endl;

// Define ROI within image bounds
int x = 50, y = 40, width = 20, height = 20;
if (x + width > image.cols || y + height > image.rows) {
  std::cerr << "Error: ROI is out of image bounds.\n";
  return -1;
}
cv::Rect roi(x, y, width, height);

// Create a view (does NOT copy data)
cv::Mat cropped_view = image(roi);

// Copy ROI to a new matrix (optional, safer)

std::cout << cropped_view.size << "\n";

// Display cropped image
cv::imshow("Cropped Image", cropped_view);
cv::waitKey(0);

// Save cropped image
if (!cv::imwrite("cropped.jpg", cropped_view)) {
  std::cerr << "Error: Failed to save cropped image.\n";
  return -1;
}

std::cout << "Cropped image saved successfully.\n";
  */

  cv::Mat image = cv::imread("/home/fw7th/Pictures/car.jpeg");
  int x1, y1, x2, y2;
  x1 = y1 = 80;
  x2 = y2 = 140;
  cv::Point pt1(x1, y1);
  cv::Point pt2(x2, y2);
  cv::rectangle(image, pt1, pt2, cv::Scalar(0, 255, 0), 4);

  cv::imshow("frame", image);
  cv::waitKey(0);
  return 0;
}
