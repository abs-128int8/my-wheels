#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cassert>
#include "math/Function.hpp"

namespace mywheels {
  template<typename Scalar>
  class Vector {
  private:
    std::vector<Scalar> m_values;

  public:
    // 初期化

    explicit Vector(std::size_t dim) : m_values(dim) {};

    Vector(std::size_t dim, Scalar val) : m_values(dim, val) {};

    Vector(std::initializer_list<Scalar> list) : m_values(list) {};


    // イテレータ

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

    // 演算子

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


    // 単項演算子
    Vector operator+() const & {
      return Vector(*this);
    }

    Vector operator+() && {
      return std::move(*this);
    }

    Vector operator-() const & {
      Vector ret(*this);
      std::transform(ret.begin(), ret.end(), ret.begin(), std::negate());
      return ret;
    }

    Vector operator-() && {
      std::transform(begin(), end(), begin(), std::negate());
      return std::move(*this);
    }

    // 複合代入演算子

    Vector &operator+=(const Vector &r) {
      assert(dim() == r.dim());
      std::transform(begin(), end(), r.begin(), begin(), std::plus());
      return *this;
    }

    Vector &operator-=(const Vector &r) {
      assert(dim() == r.dim());
      std::transform(begin(), end(), r.begin(), begin(), std::minus());
      return *this;
    }

    Vector &operator*=(const Scalar &r) {
      std::transform(begin(), end(), begin(), [&r](Scalar a) {
        return a * r;
      });
      return *this;
    }

    Vector &operator/=(const Scalar &r) {
      std::transform(begin(), end(), begin(), [&r](Scalar a) {
        return a / r;
      });
      return *this;
    }

    Vector &operator%=(const Scalar &r) {
      std::transform(begin(), end(), begin(), [&r](Scalar a) {
        return a % r;
      });
      return *this;
    }

    // 二項演算子

    friend Vector operator+(const Vector &l, const Vector &r) {
      return Vector(l) += r;
    }

    friend Vector operator+(Vector &&l, const Vector &r) {
      return l += r;
    }

    friend Vector operator+(const Vector &l, Vector &&r) {
      assert(l.dim() == r.dim());
      std::transform(l.begin(), l.end(), r.begin(), r.begin(), std::plus());
      return r;
    }

    friend Vector operator+(Vector &&l, Vector &&r) {
      return l += r;
    }

    friend Vector operator-(const Vector &l, const Vector &r) {
      return Vector(l) -= r;
    }

    friend Vector operator-(Vector &&l, const Vector &r) {
      return l -= r;
    }

    friend Vector operator-(const Vector &l, Vector &&r) {
      assert(l.dim() == r.dim());
      std::transform(l.begin(), l.end(), r.begin(), r.begin(), std::minus());
      return std::move(r);
    }

    friend Vector operator-(Vector &&l, Vector &&r) {
      return l -= r;
    }

    friend Vector operator*(const Vector &l, const Scalar &r) {
      return Vector(l) *= r;
    }

    friend Vector operator*(Vector &&l, const Scalar &r) {
      return l *= r;
    }

    friend Vector operator*(const Scalar &l, const Vector &r) {
      Vector ret(r);
      for (auto &elm : ret) {
        elm = l * elm;
      }
      return ret;
    }

    friend Vector operator*(const Scalar &l, Vector &&r) {
      for (auto &elm : r) {
        elm = l * elm;
      }
      return std::move(r);
    }

    friend Vector operator/(const Vector &l, const Scalar &r) {
      return Vector(l) /= r;
    }

    friend Vector operator/(Vector &&l, const Scalar &r) {
      return l /= r;
    }

    friend Vector operator%(const Vector &l, const Scalar &r) {
      return Vector(l) %= r;
    }

    friend Vector operator%(Vector &&l, const Scalar &r) {
      return l %= r;
    }

    // 比較演算子

    friend bool operator==(const Vector &l, const Vector &r) {
      if (l.dim() != r.dim()) {
        return false;
      }
      return std::equal(l.begin(), l.end(), r.begin());
    }

    friend bool operator!=(const Vector &l, const Vector &r) {
      return !(l == r);
    }

    // 入出力演算子
    friend std::istream &operator>>(std::istream &is, Vector &v) {
      for (auto &elm : v) {
        is >> elm;
      }
      return is;
    }

    friend std::ostream &operator<<(std::ostream &os, const Vector &v) {
      for (auto elm : v) {
        os << elm << ' ';
      }
      return os;
    }

    // 関数

    std::size_t dim() const {
      return static_cast<std::size_t>(m_values.size());
    }

    friend std::size_t dim(const Vector &v) {
      return v.dim();
    }

    Scalar dot(const Vector &r) const {
      assert(dim() == r.dim());
      return std::inner_product(begin(), end(), r.begin(), Scalar(0));
    }

    friend Scalar dot(const Vector &l, const Vector &r) {
      return l.dot(r);
    }

    Vector cross(const Vector &r) const {
      assert(dim() == 3 && r.dim() == 3);
      Vector ret = Vector::zero(3);
      ret[0] = (*this)[1] * r[2] - (*this)[2] * r[1];
      ret[1] = (*this)[2] * r[0] - (*this)[0] * r[2];
      ret[2] = (*this)[0] * r[1] - (*this)[1] * r[0];
      return ret;
    }

    friend Vector cross(const Vector &l, const Vector &r) {
      return l.cross(r);
    }

    Scalar norm() const {
      return sqrt(dot(*this));
    }

    friend Scalar norm(const Vector &v) {
      return v.norm();
    }

    Vector normalized() {
      assert(*this != Vector::zero(dim()));
      *this /= norm();
      return *this;
    }

    friend Vector normalized(const Vector &v) {
      assert(v != Vector::zero(v.dim()));
      return Vector(v) /= v.norm();
    }

    template<unsigned int P>
    Scalar lpnorm() const {
      assert(P >= 1);
      Scalar ret = std::accumulate(begin(), end(), Scalar(0), [](Scalar sum, Scalar val) {
        return sum + pow(abs(val), P);
      });
      return pow(ret, Scalar(1) / static_cast<Scalar>(P));
    }

    // 定数

    static Vector zero(std::size_t dim) {
      return Vector(dim, Scalar(0));
    }
    static Vector one(std::size_t dim) {
      return Vector(dim, Scalar(1));
    }
    static Vector stdBasis(std::size_t dim, std::size_t axis) {
      assert(axis < dim);
      Vector<Scalar> e(dim, Scalar(0));
      e(axis) = Scalar(1);
      return e;
    }
  };
} // namespace mywheels