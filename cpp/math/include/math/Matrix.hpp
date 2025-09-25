#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cassert>
#include "math/Function.hpp"
#include "math/Vector.hpp"

namespace mywheels {
  template<typename Scalar>
  class Matrix {
  private:
    std::vector<Scalar> m_values;
    std::size_t m_rows;
    std::size_t m_cols;

  public:
    // 初期化
    explicit Matrix(std::size_t dim) : m_values(dim * dim), m_rows(dim), m_cols(dim) {};

    Matrix(std::size_t rows, std::size_t cols) : m_values(rows * cols), m_rows(rows), m_cols(cols) {};

    Matrix(std::size_t dim, Scalar val) : m_values(dim * dim, val), m_rows(dim), m_cols(dim) {};

    Matrix(std::size_t rows, std::size_t cols, Scalar val) : m_values(rows * cols, val), m_rows(rows), m_cols(cols) {};

    Matrix(std::initializer_list<Scalar> list, std::size_t cols = 1) :
      m_values(list), m_rows(list.size() / cols), m_cols(cols) {
      assert(list.size() % cols == 0);
    };

    // 型変換
    operator Vector<Scalar>() const {
      assert(m_cols == 1);
      Vector<Scalar> ret(m_rows);
      std::copy(begin(), end(), ret.begin());
      return ret;
    }

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

    Scalar &operator()(std::size_t i, std::size_t j) {
      return m_values[i * m_cols + j];
    }

    const Scalar &operator()(std::size_t i, std::size_t j) const {
      return m_values[i * m_cols + j];
    }

    // 単項演算子
    Matrix operator+() const & {
      return Matrix(*this);
    }

    Matrix operator+() && {
      return std::move(*this);
    }

    Matrix operator-() const & {
      Matrix ret(*this);
      std::transform(ret.begin(), ret.end(), ret.begin(), std::negate());
      return ret;
    }

    Matrix operator-() && {
      std::transform(begin(), end(), begin(), std::negate());
      return std::move(*this);
    }

    // 複合代入演算子

    Matrix &operator+=(const Matrix &r) {
      assert(dim() == r.dim());
      std::transform(begin(), end(), r.begin(), begin(), std::plus());
      return *this;
    }

    Matrix &operator-=(const Matrix &r) {
      assert(dim() == r.dim());
      std::transform(begin(), end(), r.begin(), begin(), std::minus());
      return *this;
    }

    Matrix &operator*=(const Scalar &r) {
      std::transform(begin(), end(), begin(), [&r](Scalar a) {
        return a * r;
      });
      return *this;
    }

    Matrix &operator*=(const Matrix &r) {
      assert(m_cols == r.m_rows);
      Matrix ret = Matrix::zero(m_rows, r.m_cols);
      for (std::size_t i = 0; i < m_rows; i++) {
        for (std::size_t j = 0; j < r.m_cols; j++) {
          for (std::size_t k = 0; k < m_cols; k++) {
            ret(i, j) += (*this)(i, k) * r(k, j);
          }
        }
      }
      *this = std::move(ret);
      return *this;
    }

    Matrix &operator/=(const Scalar &r) {
      std::transform(begin(), end(), begin(), [&r](Scalar a) {
        return a / r;
      });
      return *this;
    }

    Matrix &operator%=(const Scalar &r) {
      std::transform(begin(), end(), begin(), [&r](Scalar a) {
        return a % r;
      });
      return *this;
    }

    // 二項演算子

    friend Matrix operator+(const Matrix &l, const Matrix &r) {
      return Matrix(l) += r;
    }

    friend Matrix operator+(Matrix &&l, const Matrix &r) {
      return l += r;
    }

    friend Matrix operator+(const Matrix &l, Matrix &&r) {
      assert(l.dim() == r.dim());
      std::transform(l.begin(), l.end(), r.begin(), r.begin(), std::plus());
      return r;
    }

    friend Matrix operator+(Matrix &&l, Matrix &&r) {
      return l += r;
    }

    friend Matrix operator-(const Matrix &l, const Matrix &r) {
      return Matrix(l) -= r;
    }

    friend Matrix operator-(Matrix &&l, const Matrix &r) {
      return l -= r;
    }

    friend Matrix operator-(const Matrix &l, Matrix &&r) {
      assert(l.dim() == r.dim());
      std::transform(l.begin(), l.end(), r.begin(), r.begin(), std::minus());
      return std::move(r);
    }

    friend Matrix operator-(Matrix &&l, Matrix &&r) {
      return l -= r;
    }

    friend Matrix operator*(const Matrix &l, const Scalar &r) {
      return Matrix(l) *= r;
    }

    friend Matrix operator*(Matrix &&l, const Scalar &r) {
      return l *= r;
    }

    friend Matrix operator*(const Scalar &l, const Matrix &r) {
      Matrix ret(r);
      for (auto &elm : ret) {
        elm = l * elm;
      }
      return ret;
    }

    friend Matrix operator*(const Scalar &l, Matrix &&r) {
      for (auto &elm : r) {
        elm = l * elm;
      }
      return std::move(r);
    }

    friend Matrix operator*(const Matrix &l, const Matrix &r) {
      return Matrix(l) *= r;
    }

    friend Matrix operator*(Matrix &&l, const Matrix &r) {
      return l *= r;
    }

    friend Matrix operator*(const Matrix &l, Matrix &&r) {
      assert(l.m_cols == r.m_rows);
      Matrix ret = Matrix::zero(l.m_rows, r.m_cols);
      for (std::size_t i = 0; i < l.m_rows; i++) {
        for (std::size_t j = 0; j < r.m_cols; j++) {
          for (std::size_t k = 0; k < l.m_cols; k++) {
            ret(i, j) += l(i, k) * r(k, j);
          }
        }
      }
      r = std::move(ret);
      return r;
    }

    friend Matrix operator*(Matrix &&l, Matrix &&r) {
      return l *= r;
    }

    friend Matrix operator/(const Matrix &l, const Scalar &r) {
      return Matrix(l) /= r;
    }

    friend Matrix operator/(Matrix &&l, const Scalar &r) {
      return l /= r;
    }

    friend Matrix operator%(const Matrix &l, const Scalar &r) {
      return Matrix(l) %= r;
    }

    friend Matrix operator%(Matrix &&l, const Scalar &r) {
      return l %= r;
    }

    // 比較演算子

    friend bool operator==(const Matrix &l, const Matrix &r) {
      if (l.dim() != r.dim()) {
        return false;
      }
      return std::equal(l.begin(), l.end(), r.begin());
    }

    friend bool operator!=(const Matrix &l, const Matrix &r) {
      return !(l == r);
    }

    // 入出力演算子
    friend std::istream &operator>>(std::istream &is, Matrix &v) {
      for (auto &elm : v) {
        is >> elm;
      }
      return is;
    }

    friend std::ostream &operator<<(std::ostream &os, const Matrix &v) {
      for (std::size_t i = 0; i < v.m_rows; i++) {
        for (std::size_t j = 0; j < v.m_cols; j++) {
          os << v(i, j);
          if (i != v.m_rows - 1 && j == v.m_cols - 1) {
            os << '\n';
          } else if (j != v.m_cols - 1) {
            os << ' ';
          }
        }
      }
      return os;
    }

    // 関数

    std::pair<std::size_t, std::size_t> dim() const {
      return {m_rows, m_cols};
    }

    Matrix transpose() {
      if (m_rows == m_cols) {
        for (std::size_t i = 0; i < m_rows; i++) {
          for (std::size_t j = i + 1; j < m_cols; j++) {
            std::swap((*this)(i, j), (*this)(j, i));
          }
        }
      } else {
        *this = t(*this);
      }

      return *this;
    }

    friend Matrix t(const Matrix &mat) {
      Matrix ret(mat.m_cols, mat.m_rows);
      for (std::size_t i = 0; i < mat.m_rows; i++) {
        for (std::size_t j = 0; j < mat.m_cols; j++) {
          ret(j, i) = mat(i, j);
        }
      }
      return ret;
    }

    friend Scalar tr(const Matrix &mat) {
      assert(mat.m_rows == mat.m_cols);
      Scalar ret = Scalar(0);
      for (std::size_t i = 0; i < mat.m_rows; i++) {
        ret += mat(i, i);
      }
      return ret;
    }

    Matrix row(std::size_t ix) const {
      assert(ix < m_rows);
      Matrix ret(std::size_t(1), m_cols);
      for (std::size_t j = 0; j < m_cols; j++) {
        ret(0, j) = (*this)(ix, j);
      }
      return ret;
    }

    Matrix col(std::size_t xj) const {
      assert(xj < m_cols);
      Matrix ret(m_rows, std::size_t(1));
      for (std::size_t i = 0; i < m_rows; i++) {
        ret(i, 0) = (*this)(i, xj);
      }
      return ret;
    }

    Matrix concatenateRows(const Matrix &r) const & {
      assert(m_cols == r.m_cols);
      Matrix ret(m_rows + r.m_rows, m_cols);
      std::copy(begin(), end(), ret.begin());
      std::copy(r.begin(), r.end(), ret.begin() + m_rows * m_cols);
      return ret;
    }

    Matrix concatenateRows(Matrix &&r) const & {
      assert(m_cols == r.m_cols);
      Matrix ret(m_rows + r.m_rows, m_cols);
      std::copy(begin(), end(), ret.begin());
      std::move(r.begin(), r.end(), ret.begin() + m_rows * m_cols);
      return ret;
    }

    Matrix concatenateRows(const Matrix &r) && {
      assert(m_cols == r.m_cols);
      Matrix ret(m_rows + r.m_rows, m_cols);
      std::move(begin(), end(), ret.begin());
      std::copy(r.begin(), r.end(), ret.begin() + m_rows * m_cols);
      return ret;
    }

    Matrix concatenateRows(Matrix &&r) && {
      assert(m_cols == r.m_cols);
      Matrix ret(m_rows + r.m_rows, m_cols);
      std::move(begin(), end(), ret.begin());
      std::move(r.begin(), r.end(), ret.begin() + m_rows * m_cols);
      return ret;
    }

    Matrix concatenateCols(const Matrix &r) const & {
      assert(m_rows == r.m_rows);
      Matrix ret(m_rows, m_cols + r.m_cols);
      for (std::size_t i = 0; i < m_rows; i++) {
        for (std::size_t j = 0; j < m_cols; j++) {
          ret(i, j) = (*this)(i, j);
        }
        for (std::size_t j = 0; j < r.m_cols; j++) {
          ret(i, m_cols + j) = r(i, j);
        }
      }
      return ret;
    }

    Matrix concatenateCols(Matrix &&r) const & {
      assert(m_rows == r.m_rows);
      Matrix ret(m_rows, m_cols + r.m_cols);
      for (std::size_t i = 0; i < m_rows; i++) {
        for (std::size_t j = 0; j < m_cols; j++) {
          ret(i, j) = (*this)(i, j);
        }
        for (std::size_t j = 0; j < r.m_cols; j++) {
          ret(i, m_cols + j) = std::move(r(i, j));
        }
      }
      return ret;
    }

    Matrix concatenateCols(const Matrix &r) && {
      assert(m_rows == r.m_rows);
      Matrix ret(m_rows, m_cols + r.m_cols);
      for (std::size_t i = 0; i < m_rows; i++) {
        for (std::size_t j = 0; j < m_cols; j++) {
          ret(i, j) = std::move((*this)(i, j));
        }
        for (std::size_t j = 0; j < r.m_cols; j++) {
          ret(i, m_cols + j) = r(i, j);
        }
      }
      return ret;
    }

    Matrix concatenateCols(Matrix &&r) && {
      assert(m_rows == r.m_rows);
      Matrix ret(m_rows, m_cols + r.m_cols);
      for (std::size_t i = 0; i < m_rows; i++) {
        for (std::size_t j = 0; j < m_cols; j++) {
          ret(i, j) = std::move((*this)(i, j));
        }
        for (std::size_t j = 0; j < r.m_cols; j++) {
          ret(i, m_cols + j) = std::move(r(i, j));
        }
      }
      return ret;
    }

    // idx=0の時，上からn個，左からm個を取るようにブロックに分ける
    // idx=1の時，idx=0の右のブロックを取る
    // idx=2の時，idx=0の下のブロックを取る
    // idx=3の時，idx=0の右下のブロックを取る
    Matrix block(std::size_t n, std::size_t m, std::size_t idx) const & {
      assert(n <= m_rows && m <= m_cols);
      assert(idx == 0 || idx == 1 || idx == 2 || idx == 3);
      if (idx == 0) {
        Matrix ret(n, m);
        for (std::size_t i = 0; i < n; i++) {
          for (std::size_t j = 0; j < m; j++) {
            ret(i, j) = (*this)(i, j);
          }
        }
        return ret;
      } else if (idx == 1) {
        Matrix ret(n, m_cols - m);
        for (std::size_t i = 0; i < n; i++) {
          for (std::size_t j = 0; j < m_cols - m; j++) {
            ret(i, j) = (*this)(i, m + j);
          }
        }
        return ret;
      } else if (idx == 2) {
        Matrix ret(m_rows - n, m);
        for (std::size_t i = 0; i < m_rows - n; i++) {
          for (std::size_t j = 0; j < m; j++) {
            ret(i, j) = (*this)(n + i, j);
          }
        }
        return ret;
      } else if (idx == 3) {
        Matrix ret(m_rows - n, m_cols - m);
        for (std::size_t i = 0; i < m_rows - n; i++) {
          for (std::size_t j = 0; j < m_cols - m; j++) {
            ret(i, j) = (*this)(n + i, m + j);
          }
        }
        return ret;
      }
    }

    Matrix block(std::size_t n, std::size_t m, std::size_t idx) && {
      std::cout << "hoge" << std::endl;
      assert(n <= m_rows && m <= m_cols);
      assert(idx == 0 || idx == 1 || idx == 2 || idx == 3);
      if (idx == 0) {
        Matrix ret(n, m);
        for (std::size_t i = 0; i < n; i++) {
          for (std::size_t j = 0; j < m; j++) {
            ret(i, j) = std::move((*this)(i, j));
          }
        }
        return ret;
      } else if (idx == 1) {
        Matrix ret(n, m_cols - m);
        for (std::size_t i = 0; i < n; i++) {
          for (std::size_t j = 0; j < m_cols - m; j++) {
            ret(i, j) = std::move((*this)(i, m + j));
          }
        }
        return ret;
      } else if (idx == 2) {
        Matrix ret(m_rows - n, m);
        for (std::size_t i = 0; i < m_rows - n; i++) {
          for (std::size_t j = 0; j < m; j++) {
            ret(i, j) = std::move((*this)(n + i, j));
          }
        }
        return ret;
      } else if (idx == 3) {
        Matrix ret(m_rows - n, m_cols - m);
        for (std::size_t i = 0; i < m_rows - n; i++) {
          for (std::size_t j = 0; j < m_cols - m; j++) {
            ret(i, j) = std::move((*this)(n + i, m + j));
          }
        }
        return ret;
      }
    }

    // 定数

    static Matrix zero(std::size_t rows, std::size_t cols) {
      return Matrix(rows, cols, Scalar(0));
    }

    static Matrix one(std::size_t rows, std::size_t cols) {
      return Matrix(rows, cols, Scalar(1));
    }

    static Matrix identity(std::size_t dim) {
      Matrix ret = Matrix::zero(dim, dim);
      for (std::size_t i = 0; i < dim; i++) {
        ret(i, i) = Scalar(1);
      }
      return ret;
    }
  };

  using Matf = Matrix<float>;
  using Matd = Matrix<double>;
} // namespace mywheels