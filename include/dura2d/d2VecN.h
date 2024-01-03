#ifndef VECN_H
#define VECN_H

struct d2VecN
{
    int N;
    float *data;

    d2VecN();

    d2VecN(int N);

    d2VecN(const d2VecN &v);

    ~d2VecN();

    void Zero();                               // v1.Zero()
    float Dot(const d2VecN &v) const;            // v1.Dot(v2)

    d2VecN &operator=(const d2VecN &v);          // v1 = v2
    d2VecN operator+(const d2VecN &v) const;     // v1 + v2
    d2VecN operator-(const d2VecN &v) const;     // v1 - v2
    d2VecN operator*(const float n) const;     // v1 * n
    const d2VecN &operator+=(const d2VecN &v);   // v1 += v2
    const d2VecN &operator-=(const d2VecN &v);   // v1 -= v2
    const d2VecN &operator*=(const float n);   // v1 *= n
    float operator[](const int index) const; // v1[index]
    float &operator[](const int index);      // v1[index]
};

#endif
