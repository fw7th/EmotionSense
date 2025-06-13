#include <iostream>
#include <queue>
using namespace std;

int main() {
  int value = 1337;

  int *firstValeToBePushed = &value;

  queue<int *> CheckoutLine;

  CheckoutLine.push(firstValeToBePushed);

  cout << *(CheckoutLine.front()) << "is at " << CheckoutLine.front();

  return 0;
}
