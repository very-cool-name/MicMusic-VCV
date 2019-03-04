#pragma once

#include <cmath>
#include <exception>
#include <initializer_list>
#include <string>
#include <type_traits>
#include <vector>

class IMatrix {
public:
    using size_type = int;
    using value_type = float;

    using reference = float&;
    using const_reference = const float&;

    IMatrix(size_type rows, size_type cols) : rows_(rows), cols_(cols) {}

    size_type cols() const { return cols_; }
    size_type rows() const { return rows_; }

private:
    int rows_;
    int cols_;
};

template<class Container>
class MatrixView : public IMatrix {
public:
    MatrixView(int rows, int cols, Container& container)
    : IMatrix(rows, cols), container_(container) {}

    reference operator()(size_type r, size_type c) {
        return container_[r * cols() + c];
    }

    const_reference operator()(size_type r, size_type c) const {
        return container_[r * cols() + c];
    }

private:
    Container& container_;
};

class Matrix : public IMatrix {
public:
    using size_type = int;
    using reference = float&;
    using const_reference = const float&;

    Matrix(size_type rows, size_type cols, float value = 0)
    : IMatrix(rows, cols), data_(rows * cols, value) {}

    // row-wise order
    template<class Container>
    Matrix(size_type rows, size_type cols, const Container& cont,
        typename std::enable_if<std::is_same<typename Container::value_type, float>::value>::type* = 0
    )
    : IMatrix(rows, cols), data_(rows * cols) {
        assert(rows * cols == cont.size());
        std::copy(cont.begin(), cont.end(), data_.begin());
    }

    reference operator()(size_type r, size_type c) {
        return data_[r * cols() + c];
    }

    const_reference operator()(size_type r, size_type c) const {
        return data_[r * cols() + c];
    }

private:
    std::vector<float> data_;
};

namespace helper {
    template<bool B1, bool B2>
    struct And : std::false_type {};

    template<>
    struct And<true, true> : std::true_type {};

    template<class M1, class M2>
    typename std::enable_if<
        And<
            std::is_base_of<IMatrix, M1>::value,
            std::is_base_of<IMatrix, M2>::value
        >::value
    >::type both_are_matrices(const M1& m1, const M2& m2) { }
}

template<class M1, class M2>
Matrix operator*(const M1& m1, const M2& m2) {
    helper::both_are_matrices(m1, m2);
    if (m1.cols() != m2.rows()) {
        throw std::runtime_error(
            "Mismatched dims in multiplication: "
            + std::to_string(m1.cols())
            + "x" + std::to_string(m2.rows())
        );
    }
    Matrix m(m1.rows(), m2.cols());
    for (int r = 0; r < m.rows(); ++r) {
        for (int c = 0; c < m.cols(); ++c) {
            float val = 0;
            for (int i = 0; i < m1.cols(); ++i) {
                val += m1(r, i) * m2(i, c);
            }
            m(r, c) = val;
        }
    }
    return m;
}

template<class M1, class M2>
bool operator==(const M1& m1, const M2& m2) {
    helper::both_are_matrices(m1, m2);
    if (m1.rows() != m2.rows() || m1.cols() != m2.cols())
        return false;
    for (int r = 0; r < m1.rows(); ++r) {
        for (int c = 0; c < m1.cols(); ++c) {
            if (std::abs(m1(r, c) - m2(r, c)) > 1e-6)
                return false;
        }
    }
    return true;
}

template<class M1, class M2>
bool operator!=(const M1& m1, const M2& m2) {
    return !(m1 == m2);
}

// template<class M1>
// bool operator==(const M1& m1, std::initializer_list<float>& c) {
//     if (m1.rows() * m1.cols() != c.size())
//         return false;
//     for (int r = 0; r < m1.rows(); ++r) {
//         for (int c = 0; c < m1.cols(); ++c) {
//             if (abs(m1(r, c) - c[r * m1.cols() + c]) < 1e-6)
//                 return false;
//         }
//     }
//     return true;
// }