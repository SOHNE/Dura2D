#include "dura2d/d2Math.h"

d2VecN::d2VecN() : N(0), data(nullptr)
{}

d2VecN::d2VecN(int N) : N(N)
{
    data = new real[N];
}

d2VecN::d2VecN(const d2VecN &v)
{
    N = v.N;
    data = new real[N];
    for (int i = 0; i < N; i++)
        data[i] = v.data[i];
}

d2VecN::~d2VecN()
{
    delete[] data;
}

void
d2VecN::Zero()
{
    for (int i = 0; i < N; i++)
        data[i] = 0.0f;
}

real
d2VecN::Dot(const d2VecN &v) const
{
    real sum = 0.0f;
    for (int i = 0; i < N; i++)
        sum += data[i] * v.data[i];
    return sum;
}

d2VecN &
d2VecN::operator=(const d2VecN &v)
{
    delete[] data;
    N = v.N;
    data = new real[N];
    for (int i = 0; i < N; i++)
        data[i] = v.data[i];
    return *this;
}

d2VecN
d2VecN::operator*(real n) const
{
    d2VecN result = *this;
    result *= n;
    return result;
}

d2VecN
d2VecN::operator+(const d2VecN &v) const
{
    d2VecN result = *this;
    for (int i = 0; i < N; i++)
        result.data[i] += v.data[i];
    return result;
}

d2VecN
d2VecN::operator-(const d2VecN &v) const
{
    d2VecN result = *this;
    for (int i = 0; i < N; i++)
        result.data[i] -= v.data[i];
    return result;
}

const d2VecN &
d2VecN::operator*=(real n)
{
    for (int i = 0; i < N; i++)
        data[i] *= n;
    return *this;
}

const d2VecN &
d2VecN::operator+=(const d2VecN &v)
{
    for (int i = 0; i < N; i++)
        data[i] += v.data[i];
    return *this;
}

const d2VecN &
d2VecN::operator-=(const d2VecN &v)
{
    for (int i = 0; i < N; i++)
        data[i] -= v.data[i];
    return *this;
}

real
d2VecN::operator[](const int index) const
{
    return data[index];
}

real &
d2VecN::operator[](const int index)
{
    return data[index];
}
