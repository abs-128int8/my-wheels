#include <iostream>
#include "math/Vector.hpp"

using namespace mywheels;
using namespace std;

int main() {
  Vector<double> v(5, 10.5);
  for (auto x : +Vector<double>(5, 12.1)) {
    cout << x << endl;
  }

  cout << endl;
  for (auto x : +v) {
    cout << x << endl;
  }
}