#include "dura2d/d2Constraint.h"

#include <algorithm>

///////////////////////////////////////////////////////////////////////////////
// Mat6x6 with the all inverse mass and inverse I of m_bodiesList "a" and "b"
///////////////////////////////////////////////////////////////////////////////
//  [ 1/ma  0     0     0     0     0    ]
//  [ 0     1/ma  0     0     0     0    ]
//  [ 0     0     1/Ia  0     0     0    ]
//  [ 0     0     0     1/mb  0     0    ]
//  [ 0     0     0     0     1/mb  0    ]
//  [ 0     0     0     0     0     1/Ib ]
///////////////////////////////////////////////////////////////////////////////
d2MatMN
d2Constraint::GetInvM() const
{
    d2MatMN invM(6, 6);
    invM.Zero();
    invM.rows[0][0] = a->GetInvMass();
    invM.rows[1][1] = a->GetInvMass();
    invM.rows[2][2] = a->GetInvI();
    invM.rows[3][3] = b->GetInvMass();
    invM.rows[4][4] = b->GetInvMass();
    invM.rows[5][5] = b->GetInvI();
    return invM;
}

///////////////////////////////////////////////////////////////////////////////
// d2VecN with the all linear and angular velocities of m_bodiesList "a" and "b"
///////////////////////////////////////////////////////////////////////////////
//  [ va.x ]
//  [ va.y ]
//  [ ωa   ]
//  [ vb.x ]
//  [ vb.y ]
//  [ ωb   ]
///////////////////////////////////////////////////////////////////////////////
d2VecN
d2Constraint::GetVelocities() const
{
    d2VecN V(6);
    V.Zero();
    V[0] = a->GetVelocity().x;
    V[1] = a->GetVelocity().y;
    V[2] = a->GetAngularVelocity();
    V[3] = b->GetVelocity().x;
    V[4] = b->GetVelocity().y;
    V[5] = b->GetAngularVelocity();
    return V;
}

d2JointConstraint::d2JointConstraint() : d2Constraint(), jacobian(1, 6), cachedLambda(1), bias(0.0f)
{
    cachedLambda.Zero();
}

d2JointConstraint::d2JointConstraint(d2Body *a, d2Body *b, const d2Vec2 &anchorPoint)
        : d2Constraint(), jacobian(1, 6), cachedLambda(1), bias(0.0f)
{
    this->a = a;
    this->b = b;
    this->aPoint = a->WorldSpaceToLocalSpace(anchorPoint);
    this->bPoint = b->WorldSpaceToLocalSpace(anchorPoint);
    cachedLambda.Zero();
}

void
d2JointConstraint::PreSolve(const float dt)
{
    // Get the anchor point position in world space
    const d2Vec2 pa = a->LocalSpaceToWorldSpace(aPoint);
    const d2Vec2 pb = b->LocalSpaceToWorldSpace(bPoint);

    const d2Vec2 ra = pa - a->GetPosition();
    const d2Vec2 rb = pb - b->GetPosition();

    jacobian.Zero();

    d2Vec2 J1 = (pa - pb) * 2.0;
    jacobian.rows[0][0] = J1.x; // A linear velocity.x
    jacobian.rows[0][1] = J1.y; // A linear velocity.y

    float J2 = ra.Cross(pa - pb) * 2.0;
    jacobian.rows[0][2] = J2;   // A angular velocity

    d2Vec2 J3 = (pb - pa) * 2.0;
    jacobian.rows[0][3] = J3.x; // B linear velocity.x
    jacobian.rows[0][4] = J3.y; // B linear velocity.y

    float J4 = rb.Cross(pb - pa) * 2.0;
    jacobian.rows[0][5] = J4;   // B angular velocity

    // Warm starting (apply cached lambda)
    const d2MatMN Jt = jacobian.Transpose();
    d2VecN impulses = Jt * cachedLambda;

    // Apply the impulses to both m_bodiesList
    a->ApplyImpulseLinear(d2Vec2(impulses[0], impulses[1])); // A linear impulse
    a->ApplyImpulseAngular(impulses[2]);                   // A angular impulse
    b->ApplyImpulseLinear(d2Vec2(impulses[3], impulses[4])); // B linear impulse
    b->ApplyImpulseAngular(impulses[5]);                   // B angular impulse

    // Compute the bias term (baumgarte stabilization)
    const float beta = 0.02f;
    float C = (pb - pa).Dot(pb - pa);
    C = std::max(0.0f, C - 0.01f);
    bias = (beta / dt) * C;
}

void
d2JointConstraint::Solve()
{
    const d2VecN V = GetVelocities();
    const d2MatMN invM = GetInvM();

    const d2MatMN J = jacobian;
    const d2MatMN Jt = jacobian.Transpose();

    // Compute lambda using Ax=b (Gauss-Seidel method) 
    d2MatMN lhs = J * invM * Jt;  // A
    d2VecN rhs = J * V * -1.0f;   // b
    rhs[0] -= bias;
    d2VecN lambda = d2MatMN::SolveGaussSeidel(lhs, rhs);
    cachedLambda += lambda;

    // Compute the impulses with both direction and magnitude
    d2VecN impulses = Jt * lambda;

    // Apply the impulses to both m_bodiesList
    a->ApplyImpulseLinear(d2Vec2(impulses[0], impulses[1])); // A linear impulse
    a->ApplyImpulseAngular(impulses[2]);                   // A angular impulse
    b->ApplyImpulseLinear(d2Vec2(impulses[3], impulses[4])); // B linear impulse
    b->ApplyImpulseAngular(impulses[5]);                   // B angular impulse
}

void
d2JointConstraint::PostSolve()
{
    // Limit the warm starting to reasonable limits
    cachedLambda[0] = std::clamp(cachedLambda[0], -10000.0f, 10000.0f);
}

d2PenetrationConstraint::d2PenetrationConstraint() : d2Constraint(), jacobian(2, 6), cachedLambda(2), bias(0.0f)
{
    cachedLambda.Zero();
    friction = 0.0f;
}

d2PenetrationConstraint::d2PenetrationConstraint(d2Body *a,
                                                 d2Body *b,
                                                 const d2Vec2 &aCollisionPoint,
                                                 const d2Vec2 &bCollisionPoint,
                                                 const d2Vec2 &normal)
        : d2Constraint(), jacobian(2, 6), cachedLambda(2), bias(0.0f)
{
    this->a = a;
    this->b = b;
    this->aPoint = a->WorldSpaceToLocalSpace(aCollisionPoint);
    this->bPoint = b->WorldSpaceToLocalSpace(bCollisionPoint);
    this->normal = a->WorldSpaceToLocalSpace(normal);
    cachedLambda.Zero();
    friction = 0.0f;
}

void
d2PenetrationConstraint::PreSolve(const float dt)
{
    // Get the collision points and normal in world space
    const d2Vec2 pa = a->LocalSpaceToWorldSpace(aPoint);
    const d2Vec2 pb = b->LocalSpaceToWorldSpace(bPoint);
    d2Vec2 n = a->LocalSpaceToWorldSpace(normal);

    const d2Vec2 ra = pa - a->GetPosition();
    const d2Vec2 rb = pb - b->GetPosition();

    jacobian.Zero();

    // Populate the first row of the Jacobian (normal vector)
    jacobian.rows[0][0] = -n.x;         // A linear velocity.x
    jacobian.rows[0][1] = -n.y;         // A linear velocity.y
    jacobian.rows[0][2] = -ra.Cross(n); // A angular velocity
    jacobian.rows[0][3] = n.x;          // B linear velocity.x
    jacobian.rows[0][4] = n.y;          // B linear velocity.y
    jacobian.rows[0][5] = rb.Cross(n);  // B angular velocity

    // Populate the second row of the Jacobian (tangent vector)
    friction = std::max(a->GetFriction(), b->GetFriction());
    if (friction > 0.0) {
        d2Vec2 t = n.Normal(); // The tangent is the vector perpendicular to the normal
        jacobian.rows[1][0] = -t.x;          // A linear velocity.x
        jacobian.rows[1][1] = -t.y;          // A linear velocity.y
        jacobian.rows[1][2] = -ra.Cross(t);  // A angular velocity
        jacobian.rows[1][3] = t.x;           // B linear velocity.x
        jacobian.rows[1][4] = t.y;           // B linear velocity.y
        jacobian.rows[1][5] = rb.Cross(t);   // B angular velocity
    }

    // Warm starting (apply cached lambda)
    const d2MatMN Jt = jacobian.Transpose();
    d2VecN impulses = Jt * cachedLambda;

    // Apply the impulses to both m_bodiesList
    a->ApplyImpulseLinear(d2Vec2(impulses[0], impulses[1])); // A linear impulse
    a->ApplyImpulseAngular(impulses[2]);                   // A angular impulse
    b->ApplyImpulseLinear(d2Vec2(impulses[3], impulses[4])); // B linear impulse
    b->ApplyImpulseAngular(impulses[5]);                   // B angular impulse

    // Compute the bias term (baumgarte stabilization)
    const float beta = 1.0f;
    float C = (pb - pa).Dot(-n);
    C = std::min(0.0f, C + 0.01f);
    bias = (beta / dt) * C;
}

void
d2PenetrationConstraint::Solve()
{
    // check awake state
    const d2VecN V = GetVelocities();
    const d2MatMN invM = GetInvM();

    const d2MatMN J = jacobian;
    const d2MatMN Jt = jacobian.Transpose();

    // Compute lambda using Ax=b (Gauss-Seidel method) 
    d2MatMN lhs = J * invM * Jt;  // A
    d2VecN rhs = J * V * -1.0f;   // b
    rhs[0] -= bias;
    d2VecN lambda = d2MatMN::SolveGaussSeidel(lhs, rhs);

    // Accumulate impulses and clamp it within constraint limits
    d2VecN oldLambda = cachedLambda;
    cachedLambda += lambda;
    cachedLambda[0] = (cachedLambda[0] < 0.0f) ? 0.0f : cachedLambda[0];

    // Keep friction values between -(λn*µ) and (λn*µ)
    if (friction > 0.0) {
        const float maxFriction = cachedLambda[0] * friction;
        cachedLambda[1] = std::clamp(cachedLambda[1], -maxFriction, maxFriction);
    }

    lambda = cachedLambda - oldLambda;

    // Compute the impulses with both direction and magnitude
    d2VecN impulses = Jt * lambda;

    // Apply the impulses to both m_bodiesList
    a->ApplyImpulseLinear(d2Vec2(impulses[0], impulses[1])); // A linear impulse
    a->ApplyImpulseAngular(impulses[2]);                   // A angular impulse
    b->ApplyImpulseLinear(d2Vec2(impulses[3], impulses[4])); // B linear impulse
    b->ApplyImpulseAngular(impulses[5]);                   // B angular impulse
}

void
d2PenetrationConstraint::PostSolve()
{

}
