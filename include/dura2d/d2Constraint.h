#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "d2Body.h"
#include "d2MatMN.h"

class d2Constraint
{
public:
    d2Body *a;
    d2Body *b;

    d2Vec2 aPoint; // The constraint point in A's local space
    d2Vec2 bPoint; // The constraint point in B's local space

    virtual ~d2Constraint() = default;

    d2MatMN GetInvM() const;

    d2VecN GetVelocities() const;

    virtual void PreSolve(const float dt)
    {}

    virtual void Solve()
    {}

    virtual void PostSolve()
    {}
};

class d2JointConstraint : public d2Constraint
{
private:
    d2MatMN jacobian;
    d2VecN cachedLambda;
    float bias;

public:
    d2JointConstraint();

    d2JointConstraint(d2Body *a, d2Body *b, const d2Vec2 &anchorPoint);

    void PreSolve(const float dt) override;

    void Solve() override;

    void PostSolve() override;
};

class d2PenetrationConstraint : public d2Constraint
{
private:
    d2MatMN jacobian;
    d2VecN cachedLambda;
    float bias;
    d2Vec2 normal;    // Normal direction of the penetration in A's local space
    float friction; // Friction coefficient between the two penetrating bodies

public:
    d2PenetrationConstraint();

    d2PenetrationConstraint(d2Body *a,
                            d2Body *b,
                            const d2Vec2 &aCollisionPoint,
                            const d2Vec2 &bCollisionPoint,
                            const d2Vec2 &normal);

    void PreSolve(const float dt) override;

    void Solve() override;

    void PostSolve() override;
};

#endif
