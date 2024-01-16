#ifndef D2WORLD_H
#define D2WORLD_H

#include <vector>
#include "d2api.h"
#include "./d2Body.h"
#include "./d2Constraint.h"
#include "./dura2d/d2NSquaredBroad.h"
#include "./dura2d/memory/d2BlockAllocator.h"

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
     * @brief Add a constraint to the world.
     * @param constraint The constraint to add.
     */
    void AddConstraint(d2Constraint* constraint);

    /**
     * @brief Get a reference to the list of constraints.
     * @return Reference to the list of constraints.
     */
    std::vector<d2Constraint*>& GetConstraints();

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
    int GetBodyCount() const;

private:
    d2World(const d2World& other) = delete;
    d2World& operator=(const d2World& other) = delete;

    d2Vec2 m_gravity {0.0f, -9.8f }; /**< Acceleration due to gravity. */
    std::vector<d2Constraint *> constraints; /**< List of constraints in the world. */
    std::vector<d2Vec2> forces; /**< List of external forces acting on m_bodiesList. */
    std::vector<float> torques; /**< List of external torques acting on m_bodiesList. */
    std::unique_ptr<d2Broadphase> broadphase; /**< Broad-phase collision detection algorithm. */
    d2BlockAllocator m_blockAllocator; /**< Memory m_blockAllocator for small objects. */

    d2Body* m_bodiesList { nullptr }; /**< Array of m_bodiesList in the world. */
    int m_bodyCount { 0 }; /**< Number of m_bodiesList in the world. */
};

inline d2Body* d2World::GetBodies() const   { return m_bodiesList; }

inline int d2World::GetBodyCount() const    { return m_bodyCount; }

#endif // D2WORLD_H
