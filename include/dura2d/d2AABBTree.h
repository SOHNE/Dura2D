#ifndef D2AABBTREE_H
#define D2AABBTREE_H

#include "d2Broadphase.h"
#include "d2AABB.h"

// Forward declarations
class d2Draw;

struct d2Node
{
    d2Node *parent;
    d2Node *children[2]{};
    bool childrenCrossed{};
    d2AABB aabb;
    d2AABB *data;

    d2Node(void)
            : parent(nullptr)
            , data(nullptr)
    {
        children[0] = nullptr;
        children[1] = nullptr;
    }

    inline bool IsLeaf(void) const
    {
        return children[0] == nullptr;
    }

    inline void SetBranch(d2Node *n0, d2Node *n1)
    {
        n0->parent = this;
        n1->parent = this;

        children[0] = n0;
        children[1] = n1;
    }

    inline void SetLeaf(d2AABB *data)
    {
        this->data = data;
        data->userData = this;

        children[0] = nullptr;
        children[1] = nullptr;
    }

    void UpdateAABB(float margin)
    {
        if (IsLeaf())
        {
            // Fat AABB
            const d2Vec2 marginVec(margin, margin);
            aabb.lowerBound = data->lowerBound - marginVec;
            aabb.upperBound = data->upperBound + marginVec;
        }
        else
        {
            aabb.Combine(children[0]->aabb, children[1]->aabb);
        }
    }

    inline d2Node *GetSibling(void) const
    {
        return this == parent->children[0]
                ? parent->children[1]
                : parent->children[0];
    }
};

class d2AABBTree : public d2Broadphase
{
public:

    d2AABBTree(void)
            : m_root(nullptr)
            , m_margin(2.0f)
    { }

    void Add(d2Body* body) override;
    void Remove(d2Body* body) override;
    void Update(void) override;
    ColliderPairList& ComputePairs(void) override;
    d2Body* Pick(const d2Vec2 &point) const override;
    void Query(const d2AABB &aabb, ColliderList &output) const override {}

    void DrawTree(d2Draw &draw) const;

private:

    typedef std::vector<d2Node *> NodeList;

    void UpdateNodeHelper(d2Node *node, NodeList &invalidNodes);
    void InsertNode(d2Node *node, d2Node **parent);
    void RemoveNode(d2Node *node);
    void ComputePairsHelper(d2Node *n0, d2Node *n1);
    void ClearChildrenCrossFlagHelper(d2Node *node);
    void CrossChildren(d2Node *node);

    d2Node *m_root;
    ColliderPairList m_pairs{};
    float m_margin;
    NodeList m_invalidNodes;
};

#endif //D2AABBTREE_H
