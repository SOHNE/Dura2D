#ifndef D2MATH_H
#define D2MATH_H

#include <cmath>
#include <cfloat>

#include "d2api.h"
#include "d2Types.h"

#define d2Sqrt(x) sqrtf(x)
#define d2Atan2(y, x) atan2f(y, x)
#define d2Abs(x) ((x) > 0.0F ? (x) : -(x))

#define PI 3.14159265359f
#define TAU 6.28318530718f

/**
 * @struct d2Vec2
 * @brief Represents a 2D vector.
 */
struct D2_API d2Vec2
{
    d2Vec2() : x(0.0F), y(0.0F) {}

    /**
     * @brief Constructor that initializes the vector with given x and y components.
     * @param x The x component of the vector.
     * @param y The y component of the vector.
     */
    d2Vec2(real x, real y) : x(x), y(y) {}

    ~d2Vec2() = default;

    /** @brief Sets all components of the vector to zero. */
    void Zero() { x = 0.0F; y = 0.0F; }

    /**
     * @brief Calculates the magnitude of the vector.
     * @return The magnitude of the vector.
     */
    real Lenght() const { return d2Sqrt(x * x + y * y); }

    /**
     * @brief Calculates the squared magnitude of the vector.
     * @return The squared magnitude of the vector.
     */
    real LenghtSquared() const { return x * x + y * y; }

    /**
     * @brief Normalizes the vector.
     * @return The normalized vector.
     */
    d2Vec2 &Normalize();

    /**
     * @brief Calculates the unit vector.
     * @return The unit vector.
     */
    d2Vec2 UnitVector() const
    {
        d2Vec2 result = d2Vec2(0, 0);
        real length = Lenght();
        if (length != 0.0F) {
            result.x = x / length;
            result.y = y / length;
        }
        return result;
    }

    /**
     * @brief Calculates the normal of the vector.
     * @return The normal of the vector.
     */
    d2Vec2 Normal() const { return d2Vec2(y, -x).Normalize(); }

    /**
     * @brief Calculates the dot product with another vector.
     * @param v The other vector.
     * @return The dot product.
     */
    real Dot(const d2Vec2 &v) const { return x * v.x + y * v.y; }

    /**
     * @brief Calculates the cross product with another vector.
     * @param v The other vector.
     * @return The cross product.
     */
    real Cross(const d2Vec2 &v) const { return x * v.y - y * v.x; }

    // Overloaded operators
    d2Vec2 &operator=(const d2Vec2 &v);
    bool operator==(const d2Vec2 &v) const;
    bool operator!=(const d2Vec2 &v) const;
    d2Vec2 operator+(const d2Vec2 &v) const;
    d2Vec2 operator-(const d2Vec2 &v) const;
    d2Vec2 operator*(const real n) const;
    d2Vec2 operator*(const d2Vec2 &v) const;
    d2Vec2 operator/(const real n) const;
    d2Vec2 operator-();
    d2Vec2 &operator+=(const d2Vec2 &v);
    d2Vec2 &operator-=(const d2Vec2 &v);
    d2Vec2 &operator*=(const d2Vec2 &v);
    d2Vec2 &operator*=(const real n);
    d2Vec2 &operator/=(const real n);

    real x, y; // x and y components of the vector
};

/**
 * @struct d2VecN
 * @brief Represents an N-dimensional vector.
 */
struct D2_API d2VecN
{
    d2VecN();

    /**
     * @brief Constructor that initializes the vector with given dimension.
     * @param N The dimension of the vector.
     */
    d2VecN(int N);

    /**
     * @brief Copy constructor.
     * @param v The vector to copy from.
     */
    d2VecN(const d2VecN &v);

    ~d2VecN();

    /** @brief Sets all components of the vector to zero. */
    void Zero();

    /**
     * @brief Calculates the dot product with another vector.
     * @param v The other vector.
     * @return The dot product.
     */
    real Dot(const d2VecN &v) const;

    // Overloaded operators
    d2VecN &operator=(const d2VecN &v);
    d2VecN operator+(const d2VecN &v) const;
    d2VecN operator-(const d2VecN &v) const;
    d2VecN operator*(const real n) const;
    const d2VecN &operator+=(const d2VecN &v);
    const d2VecN &operator-=(const d2VecN &v);
    const d2VecN &operator*=(const real n);
    real operator[](const int index) const;
    real &operator[](const int index);

    int N; ///< The dimension of the vector.
    real *data; ///< The data of the vector.
};

/**
 * @struct d2MatMN
 * @brief Represents an MxN matrix.
 */
struct D2_API d2MatMN
{
    d2MatMN();

    /**
     * @brief Constructor that initializes the matrix with given number of rows and columns.
     * @param M The number of rows in the matrix.
     * @param N The number of columns in the matrix.
     */
    d2MatMN(int M, int N);

    d2MatMN(const d2MatMN &m);

    ~d2MatMN();

    /** @brief Sets all components of the matrix to zero. */
    void Zero();

    /**
     * @brief Transposes the matrix.
     * @return The transposed matrix.
     */
    d2MatMN Transpose() const;

    // Overloaded operators
    const d2MatMN &operator=(const d2MatMN &m);  // Assignment operator
    d2VecN operator*(const d2VecN &v) const;     // Matrix-vector multiplication
    d2MatMN operator*(const d2MatMN &m) const;   // Matrix-matrix multiplication

    /**
     * @brief Solves a system of linear equations using the Gauss-Seidel method.
     * @param A The matrix of coefficients.
     * @param b The vector of constants.
     * @return The solution vector.
     */
    static d2VecN SolveGaussSeidel(const d2MatMN &A, const d2VecN &b);

    int M; ///< The number of rows in the matrix.
    int N; ///< The number of columns in the matrix.
    d2VecN *rows; ///< The rows of the matrix.
};

/**
 * @struct d2Rot
 * @brief Represents a rotation in 2D space.
 */
struct D2_API d2Rot
{
    /** @brief The sine of the rotation angle. */
    real s;

    /** @brief The cosine of the rotation angle. */
    real c;

    /**
     * @brief Default constructor.
     * @details Initializes the rotation to identity.
     */
    d2Rot() : s(0.0F), c(0.0F) {}

    /**
     * @brief Constructor that initializes the rotation with given angle in radians.
     * @param angle The angle in radians.
     */
    explicit d2Rot(real angle) : s(sinf(angle)), c(cosf(angle)) {}

    /**
     * @brief Constructor that initializes the rotation with given sine and cosine values.
     * @param s The sine of the rotation angle.
     * @param c The cosine of the rotation angle.
     */
    d2Rot(real s, real c) : s(s), c(c) {}

    /**
     * @brief Set the rotation angle from a given angle in radians.
     * @param angle The angle in radians.
     */
    void Set(real angle)
    {
        s = sinf(angle);
        c = cosf(angle);
    }

    /** @brief Set the rotation angle as identity. */
    void SetIdentity()
    {
        s = 0.0F;
        c = 1.0F;
    }

    /**
     * @brief Get the rotation angle in radians.
     * @return The rotation angle in radians.
     */
    inline real GetAngle() const { return d2Atan2(s, c); }

    /**
     * @brief Get the x-axis of the rotation.
     * @return The x-axis of the rotation.
     */
    inline d2Vec2 GetXAxis() const { return {c, s}; }

    /**
     * @brief Get the y-axis of the rotation.
     * @return The y-axis of the rotation.
     */
    inline d2Vec2 GetYAxis() const { return {-s, c}; }

    /**
     * @brief Adds the rotation of another d2Rot object to this object.
     * @param other The other d2Rot object whose rotation is to be added to this object.
     */
    void Add(const d2Rot& other)
    {
        real temp_s = s * other.c + c * other.s;
        real temp_c = c * other.c - s * other.s;
        s = temp_s;
        c = temp_c;
    }

    d2Rot operator+(const d2Rot& other) const
    {
        d2Rot result = *this;
        result.Add(other);
        return result;
    }

    d2Rot& operator+=(const d2Rot& other)
    {
        this->Add(other);
        return *this;
    }

    d2Rot& operator+=(real angle)
    {
        real aSin = sinf(angle);
        real aCos = cosf(angle);
        real temp_s = s * aCos + c * aSin;
        real temp_c = c * aCos - s * aSin;
        s = temp_s; c = temp_c;
        return *this;
    }
};

/**
 * @struct d2Transform
 * @brief Represents a transformation in 2D space.
 */
struct D2_API d2Transform
{
    /** @brief The translation of the body. */
    d2Vec2 p;

    /** @brief The rotation of the body. */
    d2Rot q;

    /**
     * @brief Default constructor.
     * @details Initializes the transformation to identity.
     */
    d2Transform() : p(0.0F, 0.0F), q() {}

    /**
     * @brief Constructor that initializes the transformation with given translation and rotation.
     * @param p The translation of the body.
     * @param q The rotation of the body.
     */
    d2Transform(const d2Vec2& p, const d2Rot& q) : p(p), q(q) {}

    /**
     * @brief Set the transformation as identity.
     */
    void SetIdentity()
    {
        p.Zero();
        q.SetIdentity();
    }

    /**
     * @brief Set the transformation with given translation and rotation.
     * @param p The translation of the body.
     * @param q The rotation of the body.
     */
    void Set(const d2Vec2& p, const d2Rot& q)
    {
        this->p = p;
        this->q = q;
    }
};


template <typename T>
inline T
d2Min(T a, T b)
{
    return a < b ? a : b;
}

inline d2Vec2
d2Min(const d2Vec2& a, const d2Vec2& b)
{
    return {d2Min(a.x, b.x), d2Min(a.y, b.y)};
}

template <typename T>
inline T d2Max(T a, T b)
{
    return a > b ? a : b;
}

inline d2Vec2
d2Max(const d2Vec2& a, const d2Vec2& b)
{
    return {d2Max(a.x, b.x), d2Max(a.y, b.y)};
}

template <typename T>
inline T
d2Clamp(T value, T lower, T upper)
{
    return d2Max(lower, d2Min(value, upper));
}

// Operators
inline d2Vec2
operator * (real s, const d2Vec2& v)
{
    return {s * v.x, s * v.y};
}

// Rotate a d2Vec2
inline d2Vec2
d2Rotate(const d2Rot& rot, const d2Vec2& v)
{
    real x = rot.c * v.x - rot.s * v.y;
    real y = rot.s * v.x + rot.c * v.y;
    return {x, y};
}

#endif //D2MATH_H
