#include <iostream>
#include <algorithm>
#include "deep_learning/Function.hpp"
#include "deep_learning/SimplePerceptron.hpp"
#include "math/Matrix.hpp"

using namespace mywheels;
using namespace std;

int main(int argc, char *argv[]) {
  if (argc == 1) {
    cout << "Usage: " << argv[0] << " <run_number> \n";
    cout << "\n";
    cout << "run_number\n";
    cout << "   1: SimplePerceptron\n";
    cout << "   2: 3LayerNeuralNetwork\n";
    return 0;
    return 0;
  }
  if (argc != 2) {
    return 1;
  }

  int arg = std::atoi(argv[1]);
  switch (arg) {
  case 1: {
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
    break;
  }
  case 2: {
    cout << "3LNN\n";
    Vecf x({1.0, 0.5});
    Matf W1({0.1, 0.2, 0.3, 0.4, 0.5, 0.6}, 2);
    Vecf b1({0.1, 0.2, 0.3});
    auto a1 = W1 * x + b1;
    auto z1 = a1.apply(sigmoid);
    Matf W2({0.1, 0.2, 0.3, 0.4, 0.5, 0.6}, 3);
    Vecf b2({0.1, 0.2});
    auto a2 = W2 * z1 + b2;
    auto z2 = a2.apply(sigmoid);
    Matf W3({0.1, 0.2, 0.3, 0.4}, 2);
    Vecf b3({0.1, 0.2});
    auto a3 = W3 * z2 + b3;
    auto z3 = a3;
    cout << z3 << '\n';

    cout << '\n';

    cout << "softmax\n";
    Vecf test({1010, 1000, 990});
    cout << softmax(test) << '\n';

    cout << '\n';

    cout << "hoge\n";
    cout << softmax(z3) << '\n';
    break;
  }
  case 3:
    Matf a({1, 1, 1, 1});
    cout << Matf(std::move(a), 2) << endl;
  }

  return 0;
}