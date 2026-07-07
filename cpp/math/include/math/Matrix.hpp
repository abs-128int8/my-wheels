#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cassert>
#include "math/Vector.hpp"

namespace mywheels {
  template<typename Scalar>
  class Matrix {
  private:
    std::vector<Scalar> _values;
    std::size_t _rows;
    std::size_t _cols;

  public:
    // 初期化
    explicit Matrix(std::size_t dim) : _values(dim * dim), _rows(dim), _cols(dim) {};

    Matrix(std::size_t rows, std::size_t cols) : _values(rows * cols), _rows(rows), _cols(cols) {};

    Matrix(std::size_t dim, Scalar val) : _values(dim * dim, val), _rows(dim), _cols(dim) {};

    Matrix(std::size_t rows, std::size_t cols, Scalar val) : _values(rows * cols, val), _rows(rows), _cols(cols) {};

    Matrix(std::initializer_list<Scalar> list, std::size_t cols = 1) :
      _values(list), _rows(list.size() / cols), _cols(cols) {
      assert(list.size() % cols == 0);
    };

    Matrix(const Vector<Scalar> &v) : _rows(v.dim()), _cols(std::size_t(1)), _values(v.begin(), v.end()) {};

    Matrix(Vector<Scalar> &&v) : _rows(v.dim()), _cols(std::size_t(1)), _values(v.move_values()) {};

    Matrix(const Vector<Scalar> &v, std::size_t cols) :
      _rows(v.dim() / cols), _cols(cols), _values(v.begin(), v.end()) {
      assert(v.dim() % cols == 0);
    };

    Matrix(Vector<Scalar> &&v, std::size_t cols) : _rows(v.dim() / cols), _cols(cols), _values(v.move_values()) {
      assert(v.dim() % cols == 0);
    };

    Matrix(const Matrix &m, std::size_t cols) :
      _rows(m._rows * m._cols / cols), _cols(cols), _values(m.begin(), m.end()) {
      assert(m._rows * m._cols % cols == 0);
    }

    Matrix(Matrix &&m, std::size_t cols) : _rows(m._rows * m._cols / cols), _cols(cols), _values(std::move(m._values)) {
      assert(m._rows * m._cols % cols == 0);
    }

    // 型変換
    explicit operator Vector<Scalar>() const & {
      assert(_cols == 1);
      Vector<Scalar> ret(_rows);
      std::copy(begin(), end(), ret.begin());
      return ret;
    }

    explicit operator Vector<Scalar>() && {
      assert(_cols == 1);
      Vector<Scalar> ret(_rows);
      std::move(begin(), end(), ret.begin());
      return ret;
    }

    // イテレータ

    auto begin() {
      return _values.begin();
    }

    auto end() {
      return _values.end();
    }

    auto begin() const {
      return _values.begin();
    }

    auto end() const {
      return _values.end();
    }

    // 演算子

    Scalar &operator()(std::size_t i, std::size_t j) {
      return _values[i * _cols + j];
    }

    const Scalar &operator()(std::size_t i, std::size_t j) const {
      return _values[i * _cols + j];
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
      assert(_cols == r._rows);
      Matrix ret = Matrix::zero(_rows, r._cols);
      for (std::size_t i = 0; i < _rows; i++) {
        for (std::size_t j = 0; j < r._cols; j++) {
          for (std::size_t k = 0; k < _cols; k++) {
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
      return r;
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
      return r;
    }

    friend Matrix operator*(const Matrix &l, const Matrix &r) {
      return Matrix(l) *= r;
    }

    friend Matrix operator*(Matrix &&l, const Matrix &r) {
      return l *= r;
    }

    friend Matrix operator*(const Matrix &l, Matrix &&r) {
      assert(l._cols == r._rows);
      Matrix ret = Matrix::zero(l._rows, r._cols);
      for (std::size_t i = 0; i < l._rows; i++) {
        for (std::size_t j = 0; j < r._cols; j++) {
          for (std::size_t k = 0; k < l._cols; k++) {
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
      for (std::size_t i = 0; i < v._rows; i++) {
        for (std::size_t j = 0; j < v._cols; j++) {
          os << v(i, j);
          if (i != v._rows - 1 && j == v._cols - 1) {
            os << '\n';
          } else if (j != v._cols - 1) {
            os << ' ';
          }
        }
      }
      return os;
    }

    // 関数

    std::pair<std::size_t, std::size_t> dim() const {
      return {_rows, _cols};
    }

    Matrix transpose() {
      if (_rows == _cols) {
        for (std::size_t i = 0; i < _rows; i++) {
          for (std::size_t j = i + 1; j < _cols; j++) {
            std::swap((*this)(i, j), (*this)(j, i));
          }
        }
      } else {
        *this = t(*this);
      }

      return *this;
    }

    friend Matrix t(const Matrix &mat) {
      Matrix ret(mat._cols, mat._rows);
      for (std::size_t i = 0; i < mat._rows; i++) {
        for (std::size_t j = 0; j < mat._cols; j++) {
          ret(j, i) = mat(i, j);
        }
      }
      return ret;
    }

    friend Matrix<Scalar> t(const Vector<Scalar> &v) {
      Matrix<Scalar> ret(std::size_t(1), v.dim());
      std::copy(v.begin(), v.end(), ret.begin());
      return ret;
    }

    friend Scalar tr(const Matrix &mat) {
      assert(mat._rows == mat._cols);
      Scalar ret = Scalar(0);
      for (std::size_t i = 0; i < mat._rows; i++) {
        ret += mat(i, i);
      }
      return ret;
    }

    Matrix row(std::size_t ix) const {
      assert(ix < _rows);
      Matrix ret(std::size_t(1), _cols);
      for (std::size_t j = 0; j < _cols; j++) {
        ret(0, j) = (*this)(ix, j);
      }
      return ret;
    }

    Matrix col(std::size_t xj) const {
      assert(xj < _cols);
      Matrix ret(_rows, std::size_t(1));
      for (std::size_t i = 0; i < _rows; i++) {
        ret(i, 0) = (*this)(i, xj);
      }
      return ret;
    }

    Matrix concatenateRows(const Matrix &r) const & {
      assert(_cols == r._cols);
      Matrix ret(_rows + r._rows, _cols);
      std::copy(begin(), end(), ret.begin());
      std::copy(r.begin(), r.end(), ret.begin() + _rows * _cols);
      return ret;
    }

    Matrix concatenateRows(Matrix &&r) const & {
      assert(_cols == r._cols);
      Matrix ret(_rows + r._rows, _cols);
      std::copy(begin(), end(), ret.begin());
      std::move(r.begin(), r.end(), ret.begin() + _rows * _cols);
      return ret;
    }

    Matrix concatenateRows(const Matrix &r) && {
      assert(_cols == r._cols);
      Matrix ret(_rows + r._rows, _cols);
      std::move(begin(), end(), ret.begin());
      std::copy(r.begin(), r.end(), ret.begin() + _rows * _cols);
      return ret;
    }

    Matrix concatenateRows(Matrix &&r) && {
      assert(_cols == r._cols);
      Matrix ret(_rows + r._rows, _cols);
      std::move(begin(), end(), ret.begin());
      std::move(r.begin(), r.end(), ret.begin() + _rows * _cols);
      return ret;
    }

    Matrix concatenateCols(const Matrix &r) const & {
      assert(_rows == r._rows);
      Matrix ret(_rows, _cols + r._cols);
      for (std::size_t i = 0; i < _rows; i++) {
        for (std::size_t j = 0; j < _cols; j++) {
          ret(i, j) = (*this)(i, j);
        }
        for (std::size_t j = 0; j < r._cols; j++) {
          ret(i, _cols + j) = r(i, j);
        }
      }
      return ret;
    }

    Matrix concatenateCols(Matrix &&r) const & {
      assert(_rows == r._rows);
      Matrix ret(_rows, _cols + r._cols);
      for (std::size_t i = 0; i < _rows; i++) {
        for (std::size_t j = 0; j < _cols; j++) {
          ret(i, j) = (*this)(i, j);
        }
        for (std::size_t j = 0; j < r._cols; j++) {
          ret(i, _cols + j) = std::move(r(i, j));
        }
      }
      return ret;
    }

    Matrix concatenateCols(const Matrix &r) && {
      assert(_rows == r._rows);
      Matrix ret(_rows, _cols + r._cols);
      for (std::size_t i = 0; i < _rows; i++) {
        for (std::size_t j = 0; j < _cols; j++) {
          ret(i, j) = std::move((*this)(i, j));
        }
        for (std::size_t j = 0; j < r._cols; j++) {
          ret(i, _cols + j) = r(i, j);
        }
      }
      return ret;
    }

    Matrix concatenateCols(Matrix &&r) && {
      assert(_rows == r._rows);
      Matrix ret(_rows, _cols + r._cols);
      for (std::size_t i = 0; i < _rows; i++) {
        for (std::size_t j = 0; j < _cols; j++) {
          ret(i, j) = std::move((*this)(i, j));
        }
        for (std::size_t j = 0; j < r._cols; j++) {
          ret(i, _cols + j) = std::move(r(i, j));
        }
      }
      return ret;
    }

    // idx=0の時，上からn個，左からm個を取るようにブロックに分ける
    // idx=1の時，idx=0の右のブロックを取る
    // idx=2の時，idx=0の下のブロックを取る
    // idx=3の時，idx=0の右下のブロックを取る
    Matrix block(std::size_t n, std::size_t m, std::size_t idx) const & {
      assert(n <= _rows && m <= _cols);
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
        Matrix ret(n, _cols - m);
        for (std::size_t i = 0; i < n; i++) {
          for (std::size_t j = 0; j < _cols - m; j++) {
            ret(i, j) = (*this)(i, m + j);
          }
        }
        return ret;
      } else if (idx == 2) {
        Matrix ret(_rows - n, m);
        for (std::size_t i = 0; i < _rows - n; i++) {
          for (std::size_t j = 0; j < m; j++) {
            ret(i, j) = (*this)(n + i, j);
          }
        }
        return ret;
      } else if (idx == 3) {
        Matrix ret(_rows - n, _cols - m);
        for (std::size_t i = 0; i < _rows - n; i++) {
          for (std::size_t j = 0; j < _cols - m; j++) {
            ret(i, j) = (*this)(n + i, m + j);
          }
        }
        return ret;
      }
    }

    Matrix block(std::size_t n, std::size_t m, std::size_t idx) && {
      assert(n <= _rows && m <= _cols);
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
        Matrix ret(n, _cols - m);
        for (std::size_t i = 0; i < n; i++) {
          for (std::size_t j = 0; j < _cols - m; j++) {
            ret(i, j) = std::move((*this)(i, m + j));
          }
        }
        return ret;
      } else if (idx == 2) {
        Matrix ret(_rows - n, m);
        for (std::size_t i = 0; i < _rows - n; i++) {
          for (std::size_t j = 0; j < m; j++) {
            ret(i, j) = std::move((*this)(n + i, j));
          }
        }
        return ret;
      } else if (idx == 3) {
        Matrix ret(_rows - n, _cols - m);
        for (std::size_t i = 0; i < _rows - n; i++) {
          for (std::size_t j = 0; j < _cols - m; j++) {
            ret(i, j) = std::move((*this)(n + i, m + j));
          }
        }
        return ret;
      }
    }

    template<typename Func>
    Matrix apply(Func func) const & {
      Matrix result(_rows, _cols);
      std::transform(begin(), end(), result.begin(), func);
      return result;
    }

    template<typename Func>
    Matrix apply(Func func) && {
      std::transform(begin(), end(), begin(), func);
      return *this;
    }

    Scalar max() {
      return *std::max_element(begin(), end());
    }

    Scalar min() {
      return *std::min_element(begin(), end());
    }

    Scalar sum() {
      return std::accumulate(begin(), end(), Scalar(0));
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