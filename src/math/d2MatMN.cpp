#include "dura2d/d2Math.h"

d2MatMN::d2MatMN() : M(0), N(0), rows(nullptr)
{}

d2MatMN::d2MatMN(int M, int N) : M(M), N(N)
{
    rows = new d2VecN[M];
    for (int i = 0; i < M; i++)
        rows[i] = d2VecN(N);
}

d2MatMN::d2MatMN(const d2MatMN &m)
{
    *this = m;
}

d2MatMN::~d2MatMN()
{
    delete[] rows;
}

void
d2MatMN::Zero()
{
    for (int i = 0; i < M; i++)
        rows[i].Zero();
}

d2MatMN
d2MatMN::Transpose() const
{
    d2MatMN result(N, M);
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            result.rows[j][i] = rows[i][j];

    return result;
}

const d2MatMN&
d2MatMN::operator=(const d2MatMN &m)
{
    M = m.M;
    N = m.N;
    rows = new d2VecN[M];
    for (int i = 0; i < M; i++)
        rows[i] = m.rows[i];
    return *this;
}

d2VecN
d2MatMN::operator*(const d2VecN &v) const
{
    if (v.N != N) return v;

    d2VecN result(M);
    for (int i = 0; i < M; i++)
        result[i] = v.Dot(rows[i]);

    return result;
}

d2MatMN
d2MatMN::operator*(const d2MatMN &m) const
{
    if (m.M != N && m.N != M) return m;

    d2MatMN transposed = m.Transpose();
    d2MatMN result(M, m.N);
    for (int i = 0; i < M; i++)
        for (int j = 0; j < m.N; j++)
            result.rows[i][j] = rows[i].Dot(transposed.rows[j]);

    return result;
}

d2VecN
d2MatMN::SolveGaussSeidel(const d2MatMN &A, const d2VecN &b)
{
    const int N = b.N;
    d2VecN X(N);
    X.Zero();

    // Iterate N times
    for (int iterations = 0; iterations < N; iterations++) {
        for (int i = 0; i < N; i++) {
            real dx = (b[i] / A.rows[i][i]) - (A.rows[i].Dot(X) / A.rows[i][i]);
            if (dx == dx) {
                X[i] += dx;
            }
        }
    }
    return X;
}
