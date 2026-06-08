#include <iostream>
#include <algorithm>
#include "deeplearning/Function.hpp"
#include "deeplearning/SimplePerceptron.hpp"
#include "math/Matrix.hpp"

using namespace mywheels;

int main(int argc, char *argv[]) {
  if (argc == 1) {
    std::cout << "Usage: " << argv[0] << " <run_number> \n";
    std::cout << "\n";
    std::cout << "run_number\n";
    std::cout << "   1: Chapter2: SimplePerceptron\n";
    std::cout << "   2: Chapter3: NeuralNetwork\n";
    std::cout << "\n";
    std::cout << "Ref. Oreily Deep Learning from Scratch\n";
    return 0;
  }
  if (argc != 2) {
    return 1;
  }

  int arg = std::atoi(argv[1]);
  switch (arg) {
  case 1: {
    std::cout << "1. Chapter2: SimplePerceptron\n\n";

    auto p = SimplePerceptron({0.5f, 0.5f}, -0.7f);
    std::cout << "SimplePerceptron\n";
    std::cout << p({0, 0}) << ' ' << p({0, 1}) << ' ' << p({1, 0}) << ' ' << p({1, 1}) << '\n';
    std::cout << "OR Gate\n";
    std::cout << SimplePerceptron::OR({0, 0}) << ' ' << SimplePerceptron::OR({0, 1}) << ' '
              << SimplePerceptron::OR({1, 0}) << ' ' << SimplePerceptron::OR({1, 1}) << '\n';
    std::cout << "AND Gate\n";
    std::cout << SimplePerceptron::AND({0, 0}) << ' ' << SimplePerceptron::AND({0, 1}) << ' '
              << SimplePerceptron::AND({1, 0}) << ' ' << SimplePerceptron::AND({1, 1}) << '\n';
    std::cout << "NAND Gate\n";
    std::cout << SimplePerceptron::NAND({0, 0}) << ' ' << SimplePerceptron::NAND({0, 1}) << ' '
              << SimplePerceptron::NAND({1, 0}) << ' ' << SimplePerceptron::NAND({1, 1}) << '\n';
    std::cout << "XOR Gate\n";
    std::cout << SimplePerceptron::XOR({0, 0}) << ' ' << SimplePerceptron::XOR({0, 1}) << ' '
              << SimplePerceptron::XOR({1, 0}) << ' ' << SimplePerceptron::XOR({1, 1}) << '\n';
    break;
  }
  case 2: {
    std::cout << "2. Chapter3: NeuralNetwork\n\n";
    std::cout << "3LNN forwarding\n";
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
    auto z3 = a3.apply(identity);
    std::cout << z3 << '\n';

    std::cout << '\n';

    std::cout << "softmax test\n";
    Vecf test({1010, 1000, 990});
    std::cout << "test: \n";
    std::cout << test << '\n';
    std::cout << "softmax(test): \n";
    std::cout << softmax(test) << '\n';

    break;
  }
  case 3:
    Matf a({1, 1, 1, 1});
    std::cout << Matf(std::move(a), 2) << std::endl;
  }

  return 0;
}