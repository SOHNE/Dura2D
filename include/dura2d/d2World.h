#ifndef D2WORLD_H
#define D2WORLD_H

#include <vector>

#include "d2api.h"
#include "d2Vec2.h"
#include "memory/d2BlockAllocator.h"

// Forward declarations
struct d2Color;
class d2Body;
class d2Shape;
class d2Broadphase;
class d2Constraint;
class d2Draw;

/**
 * @brief Represents a 2D physics world.
 */
class D2_API d2World
{
public:
    /**
     * @brief Constructor that initializes the world with specified gravity.
     * @param gravity The gravitational acceleration.
     */
    explicit d2World(const d2Vec2& gravity);

    /**
     * @brief Destructor to clean up resources.
     */
    ~d2World();

    /**
     * @brief Create a new rigid body with given shape, position, and mass.
     * @param shape The shape of the body.
     * @param position The initial position of the body.
     * @param mass The mass of the body.
     * @return Pointer to the created body.
     */
    d2Body* CreateBody(const d2Shape& shape, d2Vec2 position, float mass);

    /**
     * @brief Destroy a body.
     * @param body The body to destroy.
     * @warning This will automatically remove the body from the world, along with its shape.
     */
    void DestroyBody(d2Body* body);

    /**
     * @brief Create a joint between two bodies.
     * @param bodyA The first body.
     * @param bodyB The second body.
     * @param anchorPoint The anchor point for the joint.
     * @return Pointer to the created joint.
     */
    d2Constraint* CreateJoint(d2Body* bodyA, d2Body* bodyB, d2Vec2 anchorPoint);

    /**
     * @brief Destroy a joint.
     * @param joint The joint to destroy.
     * @warning This will automatically remove the joint from the world.
     */
    void DestroyJoint(d2Constraint* joint);

    /**
     * @brief Get a reference to the list of constraints.
     * @return Reference to the list of constraints.
     */
    d2Constraint*& GetConstraints();

    /**
     * @brief Get the number of constraints in the world.
     * @return The number of constraints in the world.
     */
    int32 GetConstraintCount() const;

    /**
     * @brief Add an external force to be applied to m_bodiesList.
     * @param force The force to be applied.
     */
    void AddForce(const d2Vec2& force);

    /**
     * @brief Add an external torque to be applied to m_bodiesList.
     * @param torque The torque to be applied.
     */
    void AddTorque(float torque);

    /**
     * @brief Update the world simulation by a specified time step.
     * @param dt The time step for the update.
     */
    void Update(float dt);

    /**
     * @brief Solve the world simulation by a specified time step.
     * @param dt The time step for the solve.
     */
    void Solve(float dt);

    /**
     * @brief Check for collisions between m_bodiesList.
     */
    void CheckCollisions();

    /**
     * @brief Get pointer to the array of m_bodiesList.
     * @return Pointer to the array of m_bodiesList.
     */
    d2Body* GetBodies() const;

    /**
     * @brief Get the number of m_bodiesList in the world.
     * @return The number of m_bodiesList in the world.
     */
    int32 GetBodyCount() const;

    /** @brief
     * Set the debug draw object.
     * @param debugDraw The debug draw object.
     */
    void SetDebugDraw(d2Draw* debugDraw);

    /** @brief Draw a shape. */
    void DrawShape(const d2Body* body,const d2Color& color);

    /** @brief Debug draw the world. */
    void DebugDraw();

public:
    d2World(const d2World& other) = delete;
    d2World& operator=(const d2World& other) = delete;

    d2Vec2 m_gravity {0.0f, -9.8f }; /**< Acceleration due to gravity. */
    d2Broadphase* broadphase; /**< Broad-phase collision detection algorithm. */
    d2BlockAllocator m_blockAllocator; /**< Memory m_blockAllocator for small objects. */

    d2Body* m_bodiesList { nullptr }; /**< Array of m_bodiesList in the world. */
    int32 m_bodyCount { 0 }; /**< Number of m_bodiesList in the world. */

    d2Constraint *m_constraints { nullptr }; /**< List of constraints in the world. */
    int32 m_constraintCount { 0 }; /**< Number of constraints in the world. */

    d2Draw* m_debugDraw { nullptr }; /**< Debug draw object. */
};

inline d2Body* d2World::GetBodies() const
{
    return m_bodiesList;
}

inline int32 d2World::GetBodyCount() const
{
    return m_bodyCount;
}

inline d2Constraint*& d2World::GetConstraints()
{
    return m_constraints;
}

inline int32 d2World::GetConstraintCount() const
{
    return m_constraintCount;
}

#endif // D2WORLD_H
