#include <iostream>

int main() {
  int sum = 5;
  int *ptr = &sum;
  int &ref = sum;

  std::cout << "ptr address:" << &ptr << "\n ";
  return 0;
}
