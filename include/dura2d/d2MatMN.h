#ifndef MATMN_H
#define MATMN_H

#include "dura2d/d2api.h"
#include "d2VecN.h"

struct D2_API d2MatMN
{
    int M;      // rows
    int N;      // cols
    d2VecN *rows; // the rows of the matrix with N columns inside

    d2MatMN();

    d2MatMN(int M, int N);

    d2MatMN(const d2MatMN &m);

    ~d2MatMN();

    void Zero();

    d2MatMN Transpose() const;

    const d2MatMN &operator=(const d2MatMN &m);  // m1 = m2
    d2VecN operator*(const d2VecN &v) const;     // m1 * v
    d2MatMN operator*(const d2MatMN &m) const;   // m1 * m2

    static d2VecN SolveGaussSeidel(const d2MatMN &A, const d2VecN &b);
};

#endif
