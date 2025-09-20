#include <iostream>
#include "math/Vector.hpp"

using namespace mywheels;
using namespace std;

int main() {
  Vector<double> v(2, 1);
  cout << v.lpnorm<2>() << endl;
}