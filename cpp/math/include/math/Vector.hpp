#pragma once

#include <vector>
#include <stdexcept>
#include "math/Function.hpp"

namespace mywheels {
  template<typename Scalar>
  class Vector {
  private:
    std::vector<Scalar> m_values;

  public:
    explicit Vector(std::size_t dim) : m_values(dim) {};

    Vector(std::size_t dim, Scalar val) : m_values(dim, val) {};

    Vector(std::initializer_list<Scalar> list) : m_values(list) {};


    auto begin() {
      return m_values.begin();
    }

    auto end() {
      return m_values.end();
    }

    auto begin() const {
      return m_values.begin();
    }

    auto end() const {
      return m_values.end();
    }


    Scalar &operator[](std::size_t i) {
      return m_values[i];
    }

    const Scalar &operator[](std::size_t i) const {
      return m_values[i];
    }


    Scalar &operator()(std::size_t i) {
      return m_values.at(i);
    }

    const Scalar &operator()(std::size_t i) const {
      return m_values.at(i);
    }


    Vector operator+() const & {
      return *this;
    }

    Vector operator+() && {
      return std::move(*this);
    }

    Vector operator-() const & {
      Vector ret = *this;
      for (auto &val : ret) {
        val = -val;
      }
      return ret;
    }

    Vector operator-() && {
      for (auto &val : *this) {
        val = -val;
      }
      return std::move(*this);
    }


    std::size_t dim() const {
      return static_cast<std::size_t>(m_values.size());
    }


    static Vector zero(std::size_t dim) {
      return Vector(dim, Scalar(0));
    }
    static Vector one(std::size_t dim) {
      return Vector(dim, Scalar(1));
    }
    static Vector stdBasis(std::size_t dim, std::size_t axis) {
      if (axis >= dim) {
        throw std::out_of_range("math::Vector::stdBasis: axis is out of range");
      }
      Vector<Scalar> e(dim, Scalar(0));
      e(axis) = Scalar(1);
      return e;
    }
  };
} // namespace mywheels