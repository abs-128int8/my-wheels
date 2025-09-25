#include <iostream>
#include "deep_learning/SimplePerceptron.hpp"
#include "math/Matrix.hpp"

using namespace mywheels;
using namespace std;

int main(int argc, char *argv[]) {
  if (argc == 1) {
    cout << "Usage: " << argv[0] << " <run_number> \n";
    cout << "\n";
    cout << "run_number";
    cout << "   1: SimplePerceptron\n";
    return 0;
  }
  if (argc != 2) {
    return 1;
  }

  int arg = std::atoi(argv[1]);
  switch (arg) {
  case 1:
    auto p = SimplePerceptron({0.5f, 0.5f}, -0.7f);
    cout << "SimplePerceptron\n";
    cout << p({0, 0}) << ' ' << p({0, 1}) << ' ' << p({1, 0}) << ' ' << p({1, 1}) << '\n';
    cout << "OR Gate\n";
    cout << SimplePerceptron::OR({0, 0}) << ' ' << SimplePerceptron::OR({0, 1}) << ' ' << SimplePerceptron::OR({1, 0})
         << ' ' << SimplePerceptron::OR({1, 1}) << '\n';
    cout << "AND Gate\n";
    cout << SimplePerceptron::AND({0, 0}) << ' ' << SimplePerceptron::AND({0, 1}) << ' '
         << SimplePerceptron::AND({1, 0}) << ' ' << SimplePerceptron::AND({1, 1}) << '\n';
    cout << "NAND Gate\n";
    cout << SimplePerceptron::NAND({0, 0}) << ' ' << SimplePerceptron::NAND({0, 1}) << ' '
         << SimplePerceptron::NAND({1, 0}) << ' ' << SimplePerceptron::NAND({1, 1}) << '\n';
    cout << "XOR Gate\n";
    cout << SimplePerceptron::XOR({0, 0}) << ' ' << SimplePerceptron::XOR({0, 1}) << ' '
         << SimplePerceptron::XOR({1, 0}) << ' ' << SimplePerceptron::XOR({1, 1}) << '\n';
  }

  return 0;
}