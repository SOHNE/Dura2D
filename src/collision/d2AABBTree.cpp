#include "dura2d/d2AABBTree.h"

#include "dura2d/d2Draw.h"

#include <queue>

void
d2AABBTree::Add(d2Body *body)
{
    if (!m_root)
    {
        m_root = new d2Node();
        m_root->SetLeaf(body->GetAABB());
        m_root->UpdateAABB(m_margin);
    }
    else
    {
        d2Node *node = new d2Node();
        node->SetLeaf(body->GetAABB());
        node->UpdateAABB(m_margin);
        InsertNode(node, &m_root);
    }
}

void
d2AABBTree::Remove(d2Body *body)
{
    d2Node *node = static_cast<d2Node *>(body->GetAABB()->userData);

    node->data = nullptr;
    body->GetAABB()->userData = nullptr;

    RemoveNode(node);
}

void
d2AABBTree::Update(void)
{
    if (!m_root) return;

    if (m_root->IsLeaf())
    {
        m_root->UpdateAABB(m_margin);
    }
    else
    {
        m_invalidNodes.clear();
        UpdateNodeHelper(m_root, m_invalidNodes);

        for (d2Node *node: m_invalidNodes)
        {
            d2Node *parent = node->parent;
            d2Node *sibling = node->GetSibling();
            d2Node **parentLink;

            if (parent->parent)
            {
                if (parent == parent->parent->children[0])
                {
                    parentLink = &parent->parent->children[0];
                }
                else
                {
                    parentLink = &parent->parent->children[1];
                }
            }
            else
            {
                parentLink = &m_root;
            }

            sibling->parent = parent->parent;

            *parentLink = sibling;
            delete parent;

            node->UpdateAABB(m_margin);
            InsertNode(node, &m_root);
        }
        m_invalidNodes.clear();
    }
}

void
d2AABBTree::UpdateNodeHelper(d2Node *node, NodeList &invalidNodes)
{
    if (node->IsLeaf())
    {
        if (!node->aabb.Contains(*node->data))
        {
            invalidNodes.push_back(node);
        }
    }
    else
    {
        UpdateNodeHelper(node->children[0], invalidNodes);
        UpdateNodeHelper(node->children[1], invalidNodes);
    }
}

void
d2AABBTree::InsertNode(d2Node *node, d2Node **parent)
{
    d2Node *p = *parent;
    if (p->IsLeaf())
    {
        d2Node *newParent = new d2Node();
        newParent->parent = p->parent;
        newParent->SetBranch(node, p);
        *parent = newParent;
    }
    else
    {
        const d2AABB *aabb0 = &p->children[0]->aabb;
        const d2AABB *aabb1 = &p->children[1]->aabb;
        const real volumeDiff0 = Union(*aabb0, node->aabb).GetPerimeter() - aabb0->GetPerimeter();
        const real volumeDiff1 = Union(*aabb1, node->aabb).GetPerimeter() - aabb1->GetPerimeter();

        InsertNode(node, &p->children[(volumeDiff0 < volumeDiff1 ? 0 : 1)]);
    }

    (*parent)->UpdateAABB(m_margin);
}

void
d2AABBTree::RemoveNode(d2Node *node)
{
    d2Node *parent = node->parent;
    if (parent)
    {
        d2Node *sibling = node->GetSibling();
        if (parent->parent)
        {
            sibling->parent = parent->parent;
            (parent == parent->parent->children[0]
             ? parent->parent->children[0]
             : parent->parent->children[1]) = sibling;
        }
        else
        {
            d2Node *sibling = node->GetSibling();
            m_root = sibling;
            sibling->parent = nullptr;
        }
        delete node;
        delete parent;
    }
    else // node is root
    {
        m_root = nullptr;
        delete node;
    }
}

ColliderPairList &
d2AABBTree::ComputePairs(void)
{
    m_pairs.clear();
    if (!m_root || m_root->IsLeaf()) return m_pairs;

    ClearChildrenCrossFlagHelper(m_root);

    ComputePairsHelper(m_root->children[0], m_root->children[1]);

    return m_pairs;
}

void
d2AABBTree::ClearChildrenCrossFlagHelper(d2Node *node)
{
    node->childrenCrossed = false;
    if (!node->IsLeaf())
    {
        ClearChildrenCrossFlagHelper(node->children[0]);
        ClearChildrenCrossFlagHelper(node->children[1]);
    }
}

void
d2AABBTree::CrossChildren(d2Node *node)
{
    if (!node->childrenCrossed)
    {
        ComputePairsHelper(node->children[0], node->children[1]);
        node->childrenCrossed = true;
    }
}

void
d2AABBTree::ComputePairsHelper(d2Node *n0, d2Node *n1)
{
    if (n0->IsLeaf())
    {
        if (n1->IsLeaf())
        {
            if (n0->data->Overlaps(*n1->data))
            {
                m_pairs.emplace_back(n0->data->Collider, n1->data->Collider);
            }
        }
        else
        {
            CrossChildren(n1);
            ComputePairsHelper(n0, n1->children[0]);
            ComputePairsHelper(n0, n1->children[1]);
        }
    }
    else
    {
        if (n1->IsLeaf())
        {
            CrossChildren(n0);
            ComputePairsHelper(n0->children[0], n1);
            ComputePairsHelper(n0->children[1], n1);
        }
        else
        {
            CrossChildren(n0);
            CrossChildren(n1);
            ComputePairsHelper(n0->children[0], n1->children[0]);
            ComputePairsHelper(n0->children[0], n1->children[1]);
            ComputePairsHelper(n0->children[1], n1->children[0]);
            ComputePairsHelper(n0->children[1], n1->children[1]);
        }
    }
}

d2Body*
d2AABBTree::Pick(const d2Vec2 &point) const
{
    std::queue<d2Node*> q;

    if (m_root)
        q.push(m_root);

    while (!q.empty())
    {
        d2Node &node = *q.front();
        q.pop();

        if (node.IsLeaf())
        {
            if (node.data->Contains(point))
                return node.data->Collider;
        }
        else
        {
            q.push(node.children[0]);
            q.push(node.children[1]);
        }
    }

    return nullptr;
}

void d2AABBTree::DrawTree(d2Draw &draw) const
{
    std::queue<std::pair<d2Node*, int>> q{};

    if (m_root)
        q.emplace(m_root, 0);

    while (!q.empty())
    {
        d2Node &node = *q.front().first;
        int depth = q.front().second;
        q.pop();
        if (node.IsLeaf()) continue;

        d2AABB *aabb = &node.aabb;
        d2Vec2 vertices[4] = {
            d2Vec2(aabb->lowerBound.x, aabb->lowerBound.y),
            d2Vec2(aabb->upperBound.x, aabb->lowerBound.y),
            d2Vec2(aabb->upperBound.x, aabb->upperBound.y),
            d2Vec2(aabb->lowerBound.x, aabb->upperBound.y)
        };

        // Green -> Red, where green is shallow and red is deep
        real colorFactor = depth * 0.1f;
        d2Color color = d2Color(colorFactor, 1.f - colorFactor, 0);
        draw.DrawPolygon(vertices, 4, 0, color);

        q.emplace(node.children[0], depth + 1);
        q.emplace(node.children[1], depth + 1);
    }
}
