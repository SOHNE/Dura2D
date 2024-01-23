#include "dura2d/d2World.h"

#include "dura2d/d2Body.h"
#include "dura2d/d2Shape.h"
#include "dura2d/d2AABB.h"
#include "dura2d/d2NSquaredBroad.h"
#include "dura2d/d2Constraint.h"
#include "dura2d/d2Constants.h"
#include "dura2d/d2CollisionDetection.h"
#include "dura2d/d2Draw.h"

d2World::d2World(const d2Vec2 &gravity)
{
    m_gravity = gravity * -1.0f;
    broadphase = new d2NSquaredBroad();
}

d2World::~d2World()
{
    // delete all m_bodiesList
    for (auto body = m_bodiesList; body; body = body->next) {
        body->~d2Body();
        m_blockAllocator.Free(body, sizeof(d2Body));
    }
    m_bodiesList = nullptr;

    // delete all constraints
    for (d2Constraint *constraint = m_constraints; constraint; constraint = constraint->GetNext()) {
        constraint->~d2Constraint();
        m_blockAllocator.Free(constraint, sizeof(d2Constraint));
    }
    m_constraints = nullptr;

    delete broadphase;
}

d2Body*
d2World::CreateBody(const d2Shape &shape, d2Vec2 position, float mass)
{
    void* ptr = m_blockAllocator.Allocate(sizeof(d2Body));
    d2Body* body = new(ptr) d2Body(shape, position.x, position.y, mass, this);

    // Add to world doubly linked list.
    body->prev = nullptr;
    body->next = m_bodiesList;
    if (m_bodiesList) {
        m_bodiesList->prev = body;
    }
    m_bodiesList = body;
    ++m_bodyCount;

    // add to broadphase
    broadphase->Add(body);

    return body;
}

void
d2World::DestroyBody(d2Body *body)
{
    // Remove from broadphase
    broadphase->Remove(body);

    // Remove from world doubly linked list.
    if (body->prev) {
        body->prev->next = body->next;
    }
    if (body->next) {
        body->next->prev = body->prev;
    }
    if (body == m_bodiesList) {
        m_bodiesList = body->next;
    }
    --m_bodyCount;

    // Destroy the body.
    body->~d2Body();
    m_blockAllocator.Free(body, sizeof(d2Body));
}

d2Constraint *
d2World::CreateJoint(d2Body *bodyA, d2Body *bodyB, d2Vec2 anchorPoint)
{
    void* ptr = m_blockAllocator.Allocate(sizeof(d2JointConstraint));
    d2JointConstraint* joint = new(ptr) d2JointConstraint(bodyA, bodyB, anchorPoint);

    // Add to world doubly linked list.
    joint->prev = nullptr;
    joint->next = m_constraints;
    if (m_constraints) {
        m_constraints->prev = joint;
    }
    m_constraints = joint;
    ++m_constraintCount;

    return joint;
}

void
d2World::DestroyJoint(d2Constraint *joint)
{
    // Remove from world doubly linked list.
    if (joint->prev) {
        joint->prev->next = joint->next;
    }
    if (joint->next) {
        joint->next->prev = joint->prev;
    }
    if (joint == m_constraints) {
        m_constraints = joint->next;
    }
    --m_constraintCount;

    // Destroy the joint.
    joint->~d2Constraint();
    m_blockAllocator.Free(joint, sizeof(d2Constraint));
}

d2Constraint*&
d2World::GetConstraints()
{
    return m_constraints;
}

void
d2World::AddForce(const d2Vec2 &force)
{
    forces.push_back(force);
}

void
d2World::AddTorque(float torque)
{
    torques.push_back(torque);
}

void
d2World::Update(float dt)
{
    // Create a vector of penetration constraints that will be solved frame per frame
    static std::vector<d2PenetrationConstraint> penetrations {};

    penetrations.clear();

    // Loop all m_bodiesList of the world applying forces
    for (auto body = m_bodiesList; body; body = body->next) {
        // Apply the weight force to all m_bodiesList
        d2Vec2 weight = d2Vec2(1.0, body->mass * PIXELS_PER_METER) * m_gravity;
        body->AddForce(weight);

        // Apply forces to all m_bodiesList
        for (auto& force: forces)
            body->AddForce(force);

        // Apply torque to all m_bodiesList
        for (auto& torque: torques)
            body->AddTorque(torque);

        body->IntegrateForces(dt);
        body->ComputeAABB();
    }

    {
        auto &pairs = broadphase->ComputePairs();
        for (const auto &pair: pairs) {
            auto a = pair.first;
            auto b = pair.second;

            std::vector<d2Contact> contacts{};
            if (!d2CollisionDetection::IsColliding(a, b, contacts)) continue;

            for (const auto &contact: contacts) {
                // Create a new penetration constraint
                d2PenetrationConstraint penetration(contact.a, contact.b, contact.start, contact.end, contact.normal);
                penetrations.push_back(penetration);
            }
        }
    }

    // Solve all constraints
    for (d2Constraint *constraint = m_constraints; constraint; constraint = constraint->GetNext()) {
        constraint->PreSolve(dt);
    }
    for (auto &constraint: penetrations) {
        constraint.PreSolve(dt);
    }
    for (int i = 0; i < 50; i++)
    {
        for (d2Constraint *constraint = m_constraints; constraint; constraint = constraint->GetNext()) {
            constraint->Solve();
        }
        for (auto &constraint: penetrations) {
            constraint.Solve();
        }
    }
    for (d2Constraint *constraint = m_constraints; constraint; constraint = constraint->GetNext()) {
        constraint->PostSolve();
    }
    for (auto &constraint: penetrations) {
        constraint.PostSolve();
    }

    // Integrate all the velocities
    for (auto body = m_bodiesList; body; body = body->next) {
        body->IntegrateVelocities(dt);
    }
}

void
d2World::SetDebugDraw(d2Draw *debugDraw)
{
    m_debugDraw = debugDraw;
}

void
d2World::DrawShape(const d2Body* body, const d2Color& color)
{
    if (m_debugDraw == nullptr) return;
    if (body == nullptr) return;

    d2Shape* shape = body->GetShape();
    d2Vec2 position = body->GetPosition();
    float angle = body->GetRotation();

    switch (shape->GetType())
    {
        case d2ShapeType::CIRCLE:
        {
            d2CircleShape* circle = (d2CircleShape*)shape;
            float radius = circle->radius;

            m_debugDraw->DrawSolidCircle(position, radius, angle, color);
            break;
        }
        case d2ShapeType::BOX: {
            d2BoxShape *box = (d2BoxShape *) shape;
            d2Vec2* vertices = box->worldVertices;
            int vertexCount = box->m_vertexCount;

            m_debugDraw->DrawSolidPolygon(vertices, vertexCount, angle, color);
            break;
        }
        case d2ShapeType::POLYGON:
        {
            d2PolygonShape* polygon = (d2PolygonShape*)shape;
            d2Vec2* vertices = polygon->worldVertices;
            int vertexCount = polygon->m_vertexCount;

            m_debugDraw->DrawSolidPolygon(vertices, vertexCount, angle, color);
            break;
        }
        default:
            break;
    }
}

void
d2World::DebugDraw()
{
    if (m_debugDraw == nullptr) return;

    uint32 flags = m_debugDraw->GetFlags();

    if (flags & d2Draw::e_shapeBit)
    {
        for (auto body = m_bodiesList; body; body = body->GetNext())
        {
            d2Color color;
            if (body->IsStatic())
            {
                color = d2Color(1.0f, 0.721568627f, 0.423529412f); // #ffb86c
            }
            else
            {
                color = d2Color(0.545098039f, 0.91372549f, 0.992156863f); // #8be9fd
            }

            DrawShape(body, color);
        }
    }

    if (flags & d2Draw::e_aabbBit)
    {
        for (auto body = m_bodiesList; body; body = body->GetNext())
        {
            d2AABB* aabb = body->aabb;
            d2Vec2 vertices[4] = {
                d2Vec2(aabb->minX, aabb->minY),
                d2Vec2(aabb->maxX, aabb->minY),
                d2Vec2(aabb->maxX, aabb->maxY),
                d2Vec2(aabb->minX, aabb->maxY)
            };

            m_debugDraw->DrawPolygon(vertices, 4, body->GetRotation(), d2Color(0.9f, 0.3f, 0.9f));
        }
    }

    if (flags & d2Draw::e_transformBit)
    {
        for (auto body = m_bodiesList; body; body = body->GetNext())
        {
            float angle = body->GetRotation();

            // if polygon, calculate the centroid
            if (body->GetShape()->GetType() == d2ShapeType::POLYGON || body->GetShape()->GetType() == d2ShapeType::BOX)
            {
                d2PolygonShape* polygon = (d2PolygonShape*)body->GetShape();
                d2Vec2 centroid = polygon->PolygonCentroid();

                m_debugDraw->DrawTransform(body->GetPosition() + centroid, angle);
            } else
            {
                d2Vec2 position = body->GetPosition();
                m_debugDraw->DrawTransform(position, angle);
            }
        }
    }

    if (flags & d2Draw::e_jointBit)
    {
        for (d2Constraint *constraint = m_constraints; constraint; constraint = constraint->GetNext()) {
            m_debugDraw->DrawSegment(constraint->a->GetPosition(), constraint->b->GetPosition(), d2Color(1.0f, 0.474509804f, 0.776470588f));
        }
    }
}
