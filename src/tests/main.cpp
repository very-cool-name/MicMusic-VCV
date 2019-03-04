#include "../Matrix.hpp"

#include <cassert>
#include <cmath>
#include <vector>

#define FLOAT_EQ(x, y) assert(abs(x - y) < 1e-6)

void test_default_construction() {
    Matrix m(3, 3);
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            FLOAT_EQ(m(r, c), 0);
        }
    }
}

void test_construct_from_container() {
    std::vector<float> cont = {0, 1, 2, 3, 4, 5, 6, 7};
    Matrix m(2, 4, cont);
    for (int r = 0; r < 2; ++r) {
        for (int c = 0; c < 4; ++c) {
            FLOAT_EQ(m(r, c), cont[r * 4 + c]);
        }
    }
}

void test_equality() {
    std::vector<float> cont = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    Matrix m1(4, 4, cont);
    Matrix m2(4, 4, cont);
    assert (m1 == m2);
    Matrix m3 (2, 8, cont);
    Matrix m4(8, 2, cont);
    assert(m1 != m3);
    assert(m1 != m4);
    m1(0, 0) = 20;
    assert(m1 != m2);
}

void test_mul() {
    Matrix m1(1, 3, 1);
    Matrix m2(3, 1, 1);

    Matrix m3 = m1 * m2;
    assert(m3.cols() == 1);
    assert(m3.rows() == 1);
    FLOAT_EQ(m3(0, 0), 3);

    Matrix m4 = m2 * m1;
    assert(m4.cols() == 3);
    assert(m4.rows() == 3);
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            FLOAT_EQ(m4(r, c), 1);
        }
    }
}

int main(){
    test_default_construction();
    test_construct_from_container();
    test_equality();
    test_mul();
}