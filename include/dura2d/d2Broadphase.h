#ifndef DURA2D_D2BROADPHASE_H
#define DURA2D_D2BROADPHASE_H

#include <memory>
#include <utility>
#include <list>

#include "dura2d/d2Body.h"

typedef std::pair<d2Body*, d2Body*> ColliderPair;
typedef std::list<ColliderPair> ColliderPairList;

class d2Broadphase
{
public:

    virtual ~d2Broadphase() = default;

    // adds a new d2AABB to the broadphase
    virtual void Add(d2Body* body) = 0;

    // removes a d2AABB from the broadphase
    virtual void Remove(d2Body* body) = 0;

    // updates broadphase to react to changes to d2AABB
    virtual void Update(void) = 0;

    // returns a list of possibly colliding colliders
    virtual const ColliderPairList &ComputePairs(void) = 0;

    // returns a collider that collides with a point
    // returns null if no such collider exists
    virtual d2Body *Pick(const d2Vec2 &point) const = 0;

    // returns a list of colliders whose AABBs collide
    // with a query d2AABB
    typedef std::vector<d2Body *> ColliderList;
    virtual void Query(const d2AABB &aabb, ColliderList &output) const = 0;
};


#endif //DURA2D_D2BROADPHASE_H
