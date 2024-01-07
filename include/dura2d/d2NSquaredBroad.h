#ifndef DURA2D_D2NSQUAREDBROAD_H
#define DURA2D_D2NSQUAREDBROAD_H

#include "dura2d/d2Broadphase.h"

class d2NSquaredBroad : public d2Broadphase
{
public:
        // adds a new AABB to the broadphase
        void Add(d2Body* body) override;

        // updates broadphase to react to changes to AABB
        void Update(void) override;

        // returns a list of possibly colliding colliders
        const ColliderPairList &ComputePairs(void) override;

        // returns a collider that collides with a point
        // returns null if no such collider exists
        d2Body *Pick(const d2Vec2 &point) const override;

        // returns a list of colliders whose AABBs collide
        // with a query AABB
        typedef std::vector<d2Body *> ColliderList;
        void Query(const AABB &aabb, ColliderList &output) const override;

private:
        std::vector<d2Body *> bodies{};
        ColliderPairList m_pairs{};
};

#endif //DURA2D_D2NSQUAREDBROAD_H
