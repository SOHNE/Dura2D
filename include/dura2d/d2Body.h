#ifndef BODY_H
#define BODY_H

#include "d2api.h"

#include "d2Shape.h"
#include "d2Math.h"
#include "d2Types.h"

// Forward declarations
class d2World;
class d2AABB;

// enums
enum d2BodyType
{
    d2_staticBody = 0,      //< A static body does not move or rotate.
    d2_dynamicBody      //< A dynamic body is fully simulated.
};

/**
 * @class d2Body
 * @brief A class representing a 2D rigid body.
 *
 * This class encapsulates the properties and behaviors of a 2D rigid body.
 */
class D2_API d2Body
{
public:
    /**
     * @brief Constructor for the d2Body class.
     *
     * @param shape The shape/geometry of the body.
     * @param x The initial x-coordinate of the body.
     * @param y The initial y-coordinate of the body.
     * @param mass The mass of the body.
     */
    d2Body(const d2Shape &shape, float x, float y, float mass, d2World* world);

    /**
     * @brief Destructor for the d2Body class.
     */
    ~d2Body();

    /** @brief Computes the Axis-Aligned Bounding Box of the body. */
    void ComputeAABB();

    /**
     * @brief Adds a force to the body.
     *
     * @param force The force to be added.
     */
    void AddForce(const d2Vec2 &force);

    /**
     * @brief Adds a torque to the body.
     *
     * @param torque The torque to be added.
     */
    void AddTorque(float torque);

    /** @brief Clears all forces applied to the body. */
    void ClearForces();

    /** @brief Clears all torques applied to the body. */
    void ClearTorque();

    /**
     * @brief Converts a point from local space to world space.
     *
     * @param point The point in local space.
     * @return The point in world space.
     */
    d2Vec2 LocalSpaceToWorldSpace(const d2Vec2 &point) const;

    /**
     * @brief Converts a point from world space to local space.
     *
     * @param point The point in world space.
     * @return The point in local space.
     */
    d2Vec2 WorldSpaceToLocalSpace(const d2Vec2 &point) const;

    /**
     * @brief Applies a linear impulse to the body.
     *
     * @param j The impulse to be applied.
     */
    void ApplyImpulseLinear(const d2Vec2 &j);

    /**
     * @brief Applies an angular impulse to the body.
     *
     * @param j The impulse to be applied.
     */
    void ApplyImpulseAngular(const float j);

    /**
     * @brief Applies an impulse at a specific point on the body.
     *
     * @param j The impulse to be applied.
     * @param r The point at which the impulse is applied.
     */
    void ApplyImpulseAtPoint(const d2Vec2 &j, const d2Vec2 &r);

    /**
     * @brief Integrates the linear motion of the body over a time step.
     *
     * @param dt The time step.
     */
    void IntegrateLinear(float dt);

    /**
     * @brief Integrates the angular motion of the body over a time step.
     *
     * @param dt The time step.
     */
    void IntegrateAngular(float dt);

    /**
     * @brief Integrates the forces applied to the body over a time step.
     *
     * @param dt The time step.
     */
    void IntegrateForces(const float dt);

    /**
     * @brief Integrates the velocities of the body over a time step.
     *
     * @param dt The time step.
     */
    void IntegrateVelocities(const float dt);

    /**
     * @brief Gets the position of the body.
     *
     * @return The position of the body.
     */
    inline const d2Vec2& GetPosition() const;

    /**
     * @brief Sets the position of the body.
     *
     * @param position The position of the body.
     */
    inline void SetPosition(const d2Vec2& position);

    /**
     * @brief Gets the velocity of the body.
     *
     * @return The velocity of the body.
     */
    inline const d2Vec2& GetVelocity() const;

    /** @brief Set acceleration of the body. */
    inline void SetAcceleration(const d2Vec2& acceleration);

    /** @brief Sets the angular velocity of the body. */
    inline void SetAngularVelocity(float angularVelocity);

    /**
     * @brief Gets the angular velocity of the body.
     *
     * @return The angular velocity of the body.
     */
    inline float GetAngularVelocity() const;

    /**
     * @brief Gets the rotation angle of the body.
     *
     * @return The rotation angle of the body.
     */
    inline float GetRotation() const;

    /**
     * @brief Gets the mass of the body.
     *
     * @return The mass of the body.
     */
    inline float GetMass() const;

    /**
     * @brief Gets the inverse mass of the body.
     *
     * @return The inverse mass of the body.
     */
    inline float GetInvMass() const;

    /**
     * @brief Gets the moment of inertia of the body.
     *
     * @return The moment of inertia of the body.
     */
    inline float GetI() const;

    /**
     * @brief Gets the inverse moment of inertia of the body.
     *
     * @return The inverse moment of inertia of the body.
     */
    inline float GetInvI() const;

    /** @brief Sets the coefficient of friction of the body. */
    inline void SetFriction(float friction);

    /**
     * @brief Gets the coefficient of friction of the body.
     *
     * @return The coefficient of friction of the body.
     */
    inline float GetFriction() const;

    /** @brief Sets the coefficient of restitution of the body. */
    inline void SetRestitution(float restitution);

    /**
     * @brief Gets the coefficient of restitution of the body.
     *
     * @return The coefficient of restitution of the body.
     */
    inline float GetRestitution() const;

    /**
     * @brief Gets the shape/geometry of the body.
     *
     * @return A pointer to the shape/geometry of the body.
     */
    inline d2Shape* GetShape() const;

    /**
     * @brief Gets the Axis-Aligned Bounding Box of the body.
     *
     * @return A pointer to the Axis-Aligned Bounding Box of the body.
     */
    inline d2AABB* GetAABB() const;

    /**
     * @brief Gets the next body in a linked list of bodies.
     *
     * @return A pointer to the next body in a linked list of bodies.
     */
    inline d2Body* GetNext();
    inline const d2Body* GetNext() const;

    /**
     * @brief Gets the previous body in a linked list of bodies.
     *
     * @return A pointer to the previous body in a linked list of bodies.
     */
    inline d2Body* GetPrev();
    inline const d2Body* GetPrev() const;

    /**
     * @brief Gets wake-up time of the body.
     *
     * @return The wake-up time of the body.
     */
    inline float GetSleepTime() const;

    /**
     * @brief Gets the wake state of the body.
     *
     * @return The wake state of the body.
     */
    inline bool IsAwake() const;

    /**
     * @brief Sets the wake state of the body.
     *
     * @param awake The wake state of the body.
     */
    inline void SetAwake(bool awake);

    /** @brief Gets the gravity scale of the body. */
    inline float GetGravityScale() const;

    /** @brief Sets the gravity scale of the body */
    inline void SetGravityScale(float gravityScale);

private:
    friend class d2World;

    enum
    {
        e_awakeFlag = 0x0001
    };

    uint16 m_flags{};
    d2BodyType m_type{};

    d2Transform m_transform; ///< The transformation of the body.

    d2Vec2 velocity {}; ///< The velocity of the body.
    d2Vec2 acceleration {}; ///< The acceleration of the body.

    float m_gravityScale{ 1.0f }; ///< The gravity scale of the body.

    float angularVelocity {}; ///< The angular velocity of the body.
    float angularAcceleration {}; ///< The angular acceleration of the body.

    d2Vec2 sumForces {}; ///< The sum of all forces applied to the body.
    float sumTorque {}; ///< The sum of all torques applied to the body.

    float mass {}; ///< The mass of the body.
    float invMass {}; ///< The inverse of the mass of the body.
    float I {}; ///< The moment of inertia of the body.
    float invI {}; ///< The inverse of the moment of inertia of the body.

    float restitution {}; ///< The coefficient of restitution (elasticity) of the body.

    float friction {}; ///< The coefficient of friction of the body.

    d2AABB *aabb { nullptr }; ///< A pointer to the Axis-Aligned Bounding Box of the body.

    d2Shape *shape { nullptr }; ///< A pointer to the shape/geometry of the body.

    d2World* world { nullptr }; ///< A pointer to the world that this body belongs to.
    d2Body* prev { nullptr }; ///< A pointer to the previous body in a linked list of bodies.
    d2Body* next { nullptr }; ///< A pointer to the next body in a linked list of bodies.

    float m_sleepTime{}; ///< The time that the body has been stationary.
};

inline const d2Vec2& d2Body::GetPosition() const
{
    return m_transform.p;
}

inline void d2Body::SetPosition(const d2Vec2& position)
{
    m_transform.p = position;
}

inline const d2Vec2& d2Body::GetVelocity() const
{
    return velocity;
}

inline void d2Body::SetAcceleration(const d2Vec2& acceleration)
{
    this->acceleration = acceleration;
}

inline void d2Body::SetAngularVelocity(float angularVelocity)
{
    this->angularVelocity = angularVelocity;
}

inline float d2Body::GetAngularVelocity() const
{
    return angularVelocity;
}

inline float d2Body::GetRotation() const
{
    return m_transform.q.GetAngle();
}

inline float d2Body::GetMass() const
{
    return mass;
}

inline float d2Body::GetInvMass() const
{
    return invMass;
}

inline float d2Body::GetI() const
{
    return I;
}

inline float d2Body::GetInvI() const
{
    return invI;
}

inline void d2Body::SetFriction(float friction)
{
    this->friction = friction;
}

inline float d2Body::GetFriction() const
{
    return friction;
}

inline void d2Body::SetRestitution(float restitution)
{
    this->restitution = restitution;
}

inline float d2Body::GetRestitution() const
{
    return restitution;
}

inline d2Shape* d2Body::GetShape() const
{
    return shape;
}

inline d2AABB* d2Body::GetAABB() const
{
    return aabb;
}

inline d2Body* d2Body::GetNext()
{
    return next;
}

inline const d2Body* d2Body::GetNext() const
{
    return next;
}

inline d2Body* d2Body::GetPrev()
{
    return prev;
}

inline const d2Body* d2Body::GetPrev() const
{
    return prev;
}

inline float d2Body::GetSleepTime() const
{
    return m_sleepTime;
}

inline float d2Body::GetGravityScale() const
{
    return m_gravityScale;
}

inline void d2Body::SetGravityScale(float gravityScale)
{
    m_gravityScale = gravityScale;
}

inline bool d2Body::IsAwake() const
{
    return (m_flags & e_awakeFlag) == e_awakeFlag;
}

inline void d2Body::SetAwake(bool awake)
{
    if (awake)
    {
        m_flags |= e_awakeFlag;
        m_sleepTime = 0.0f;
    }
    else
    {
        m_flags &= ~e_awakeFlag;
        m_sleepTime = 0.0f;
        velocity = d2Vec2(0, 0);
        angularVelocity = 0.0f;
        sumForces = d2Vec2(0, 0);
        sumTorque = 0.0f;
    }
}

#endif