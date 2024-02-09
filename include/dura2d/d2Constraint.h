#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "d2Body.h"

class d2Constraint
{
public:
    d2Body *a;
    d2Body *b;

    d2Vec2 aPoint; // The constraint point in A's local space
    d2Vec2 bPoint; // The constraint point in B's local space

    d2Constraint* next;
    d2Constraint* prev;

    virtual ~d2Constraint() = default;

    d2MatMN GetInvM() const;

    d2VecN GetVelocities() const;

    virtual void PreSolve(const real dt) {}

    virtual void Solve() {}

    virtual void PostSolve() {}

    void SetNext(d2Constraint* next) { this->next = next; }

    void SetPrev(d2Constraint* prev) { this->prev = prev; }

    d2Constraint* GetNext() { return next; }
    const d2Constraint* GetNext() const { return next; }

    d2Constraint* GetPrev() { return prev; }
    const d2Constraint* GetPrev() const { return prev; }
};

class d2JointConstraint : public d2Constraint
{
private:
    d2MatMN jacobian;
    d2VecN cachedLambda;
    real bias;

public:
    d2JointConstraint();

    d2JointConstraint(d2Body *a, d2Body *b, const d2Vec2 &anchorPoint);

    void PreSolve(const real dt) override;

    void Solve() override;

    void PostSolve() override;
};

class d2PenetrationConstraint : public d2Constraint
{
private:
    d2MatMN jacobian;
    d2VecN cachedLambda;
    real bias;
    d2Vec2 normal;    // Normal direction of the penetration in A's local space
    real friction; // Friction coefficient between the two penetrating m_bodiesList

public:
    d2PenetrationConstraint();

    d2PenetrationConstraint(d2Body *a,
                            d2Body *b,
                            const d2Vec2 &aCollisionPoint,
                            const d2Vec2 &bCollisionPoint,
                            const d2Vec2 &normal);

    void PreSolve(const real dt) override;

    void Solve() override;

    void PostSolve() override;
};

#endif
