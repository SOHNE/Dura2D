#include "dura2d/d2World.h"

#include "dura2d/d2Body.h"
#include "dura2d/d2AABB.h"
#include "dura2d/d2NSquaredBroad.h"
#include "dura2d/d2AABBTree.h"
#include "dura2d/d2Constraint.h"
#include "dura2d/d2Constants.h"
#include "dura2d/d2CollisionDetection.h"
#include "dura2d/d2Draw.h"

#include "dura2d/d2Timer.h"

#include <iostream>

d2World::d2World(const d2Vec2 &gravity)
{
    m_gravity = gravity * -1.0f;
    broadphase = new d2AABBTree();
}

d2World::~d2World()
{
    delete broadphase;
}

d2Body*
d2World::CreateBody(const d2Shape &shape, d2Vec2 position, real mass)
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

void
d2World::Step(real dt, int32 posIterations)
{
    // Loop all m_bodiesList of the world applying forces
    for (auto body = m_bodiesList; body; body = body->next)
    {
        if (body->m_type == d2BodyType::d2_staticBody) continue;

        const real massScaled = body->mass * PIXELS_PER_METER * body->m_gravityScale;
        body->AddForce(m_gravity * massScaled);

        body->IntegrateForces(dt);
        body->ComputeAABB();
    }

    broadphase->Update();

    std::vector<d2PenetrationConstraint> penetrations;
    {
        //d2Timer timer;

        ColliderPairList pairs;
        pairs = broadphase->ComputePairs();
        for (const auto &pair: pairs)
        {
            auto a = pair.first;
            auto b = pair.second;

            std::vector<d2Contact> contacts;
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
    for (auto& penetration: penetrations) {
        penetration.PreSolve(dt);
    }
    for (int i = 0; i < posIterations; i++)
    {
        for (d2Constraint *constraint = m_constraints; constraint; constraint = constraint->GetNext()) {
            constraint->Solve();
        }
        for (auto& penetration: penetrations) {
            penetration.Solve();
        }
    }
    for (d2Constraint *constraint = m_constraints; constraint; constraint = constraint->GetNext()) {
        constraint->PostSolve();
    }
    for (auto& penetration: penetrations) {
        penetration.PostSolve();
    }

    // Integrate all the velocities
    for (auto body = m_bodiesList; body; body = body->next) {
        if (body->m_type == d2BodyType::d2_staticBody) continue;

        body->IntegrateVelocities(dt);
    }
}

void
d2World::SetDebugDraw(d2Draw *debugDraw)
{
    m_debugDraw = debugDraw;
}

void
d2World::DrawShape(const d2Body* body, const bool &mesh, const d2Color& color)
{
    d2Shape* shape = body->GetShape();
    d2Vec2 position = body->GetPosition();
    real angle = body->GetRotation();

    switch (shape->GetType())
    {
        case d2ShapeType::CIRCLE:
        {
            d2CircleShape *circle = (d2CircleShape*)shape;
            real radius = circle->radius;

            m_debugDraw->DrawSolidCircle(position, radius, angle, color);
            break;
        }
        case d2ShapeType::BOX:
        case d2ShapeType::POLYGON:
        {
            d2PolygonShape *polygon = (d2PolygonShape*)shape;
            d2Vec2 *vertices = polygon->worldVertices;
            int vertexCount = polygon->m_vertexCount;

            m_debugDraw->DrawSolidPolygon(vertices, vertexCount, angle, mesh, color);
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
        d2Color staticColor(1.0f, 0.721568627f, 0.423529412f); // #ffb86c
        d2Color dynamicColor(0.545098039f, 0.91372549f, 0.992156863f); // #8be9fd
        bool mesh = flags & d2Draw::e_meshBit;

        for (d2Body *b = m_bodiesList; b; b = b->GetNext())
        {
            d2Color color = b->m_type == d2BodyType::d2_staticBody ? staticColor : dynamicColor;
            DrawShape(b, mesh, color);
        }
    }

    if (flags & d2Draw::e_aabbBit)
    {
        d2Color color(0.9f, 0.3f, 0.9f);

        for (d2Body *b = m_bodiesList; b; b = b->GetNext())
        {
            d2AABB *aabb = b->aabb;
            d2Vec2 vertices[4] = {
                    d2Vec2(aabb->lowerBound.x, aabb->lowerBound.y),
                    d2Vec2(aabb->upperBound.x, aabb->lowerBound.y),
                    d2Vec2(aabb->upperBound.x, aabb->upperBound.y),
                    d2Vec2(aabb->lowerBound.x, aabb->upperBound.y)
            };

            m_debugDraw->DrawPolygon(vertices, 4, b->GetRotation(), color);
        }
    }

    if (flags & d2Draw::e_aabbTreeBit)
    {
        // check if broadphase is d2AABBTree
        if (auto tree = dynamic_cast<d2AABBTree*>(broadphase))
        {
            tree->DrawTree(*m_debugDraw);
        }
    }

    if (flags & d2Draw::e_transformBit)
    {
        for (d2Body *b = m_bodiesList; b; b = b->GetNext())
        {
            d2ShapeType sType = b->GetShape()->GetType();
            d2Transform transform = b->m_transform;

            switch (sType)
            {
                case d2ShapeType::POLYGON:
                case d2ShapeType::BOX:
                {
                    d2PolygonShape *polygon = dynamic_cast<d2PolygonShape*>(b->GetShape());
                    transform.p += polygon->PolygonCentroid();
                    break;
                }
                default:
                    break;
            }

            m_debugDraw->DrawTransform(transform);
        }
    }

    if (flags & d2Draw::e_jointBit)
    {
        d2Color color(1.0f, 0.474509804f, 0.776470588f); // #ff79c6
        for (d2Constraint *constraint = m_constraints; constraint; constraint = constraint->GetNext())
        {
            m_debugDraw->DrawSegment(constraint->a->GetPosition(), constraint->b->GetPosition(), color);
        }
    }
}
