#include "dura2d/d2NSquaredBroad.h"

#include "dura2d/d2Body.h"
#include "dura2d/d2AABB.h"

void
d2NSquaredBroad::Add(d2Body *body)
{
    bodies.push_back(body);
}

void
d2NSquaredBroad::Update(void)
{

}

const ColliderPairList &
d2NSquaredBroad::ComputePairs(void)
{
    m_pairs.clear();

    for (auto i = 0; i < bodies.size(); ++i) {
        for (auto j = i + 1; j < bodies.size(); ++j) {
            AABB *a = bodies[i]->GetAABB();
            AABB *b = bodies[j]->GetAABB();

            if (bodies[i] == bodies[j]) continue;

            if (a->Overlaps(*b)) {
                m_pairs.emplace_back(bodies[i], bodies[j]);
            }
        }
    }

    return m_pairs;
}

d2Body *
d2NSquaredBroad::Pick(const d2Vec2 &point) const
{
    for (const auto &body : bodies) {
        if (body->GetAABB()->Contains(point)) {
            return body;
        }
    }
    return nullptr;
}

void
d2NSquaredBroad::Query(const AABB &aabb, d2NSquaredBroad::ColliderList &output) const
{
    for (const auto &body : bodies) {
        if (body->GetAABB()->Overlaps(aabb)) {
            output.push_back(body);
        }
    }
}
