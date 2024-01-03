#ifndef DURA2D_D2BROADPHASE_H
#define DURA2D_D2BROADPHASE_H

#include <utility>
#include <list>

#include "dura2d/d2Vec2.h"
#include "dura2d/d2Body.h"

class AABB;

typedef std::pair<std::shared_ptr<d2Body>, std::shared_ptr<d2Body>> ColliderPair;
typedef std::list<ColliderPair> ColliderPairList;

class d2Broadphase
{
public:

    virtual ~d2Broadphase() = default;

    // adds a new AABB to the broadphase
    virtual void Add(std::shared_ptr<d2Body> body) = 0;

    // updates broadphase to react to changes to AABB
    virtual void Update(void) = 0;

    // returns a list of possibly colliding colliders
    virtual const ColliderPairList &ComputePairs(void) = 0;

    // returns a collider that collides with a point
    // returns null if no such collider exists
    virtual d2Body *Pick(const d2Vec2 &point) const = 0;

    // returns a list of colliders whose AABBs collide
    // with a query AABB
    typedef std::vector<d2Body *> ColliderList;
    virtual void Query(const AABB &aabb, ColliderList &output) const = 0;
};


#endif //DURA2D_D2BROADPHASE_H
