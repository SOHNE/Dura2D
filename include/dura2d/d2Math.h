#ifndef D2MATH_H
#define D2MATH_H

#include <cmath>

#include "d2api.h"
#include "d2Vec2.h"

#define d2Sqrt(x) sqrtf(x)
#define d2Atan2(y, x) atan2f(y, x)
#define d2Abs(x) ((x) > 0.0F ? (x) : -(x))
#define d2Min(a, b) ((a) < (b) ? (a) : (b))
#define d2Max(a, b) ((a) > (b) ? (a) : (b))
#define d2Clamp(a, low, high) (d2Max(low, d2Min(a, high)))

#define PI 3.14159265359f
#define TAU 6.28318530718f

/** @brief Represents a rotation of a body in 2D space. */
struct D2_API d2Rot
{
    /** @brief The sine of the rotation angle. */
    float s;

    /** @brief The cosine of the rotation angle. */
    float c;

    /**
     * @brief Default constructor.
     * @details Initializes the rotation to identity.
     */
    d2Rot() : s(0.0F), c(0.0F) {}

    /**
     * @brief Constructor that initializes the rotation with given angle in radians.
     * @param angle The angle in radians.
     */
    explicit d2Rot(float angle) : s(sinf(angle)), c(cosf(angle)) {}

    /**
     * @brief Constructor that initializes the rotation with given sine and cosine values.
     * @param s The sine of the rotation angle.
     * @param c The cosine of the rotation angle.
     */
    d2Rot(float s, float c) : s(s), c(c) {}

    /**
     * @brief Set the rotation angle from a given angle in radians.
     * @param angle The angle in radians.
     */
    void Set(float angle)
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
    float GetAngle() const { return d2Atan2(s, c); }

    /**
     * @brief Get the x-axis of the rotation.
     * @return The x-axis of the rotation.
     */
    d2Vec2 GetXAxis() const { return {c, s}; }

    /**
     * @brief Get the y-axis of the rotation.
     * @return The y-axis of the rotation.
     */
    d2Vec2 GetYAxis() const { return {-s, c}; }

    /**
     * @brief Adds the rotation of another d2Rot object to this object.
     * @param other The other d2Rot object whose rotation is to be added to this object.
     */
    void Add(const d2Rot& other)
    {
        float temp_s = s * other.c + c * other.s;
        float temp_c = c * other.c - s * other.s;
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

    d2Rot& operator+=(float angle)
    {
        this->Add(d2Rot(angle));
        return *this;
    }
};

/** @brief Represents a transformation of a body in 2D space. */
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
        p = d2Vec2(0.0F, 0.0F);
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

#endif //D2MATH_H
