#ifndef DRAW_H
#define DRAW_H

#include "dura2d/dura2d.h"

#include "raylib.h"

class draw : public d2Draw
{
public:
    draw();
    ~draw();

    void DrawPolygon(const d2Vec2 *vertices, int32 vertexCount, const float &angle, const d2Color &color) override;

    void DrawSolidPolygon(const d2Vec2 *vertices, int32 vertexCount, const float &angle, const d2Color &color) override;

    void DrawCircle(const d2Vec2 &center, float radius, const float &angle, const d2Color &color) override;

    void DrawSolidCircle(const d2Vec2 &center, float radius, const float &angle, const d2Color &color) override;

    void DrawTransform(const d2Transform &transform) override;

    void DrawSegment(const d2Vec2 &p1, const d2Vec2 &p2, const d2Color &color) override;

public:
    uint32 screenWidth{800};
    uint32 screenHeight{600};

    uint32 m_flags{e_shapeBit};

private:
    Color convertToColor(const d2Color &color);
};

// Externs
extern draw g_draw;

#endif //DRAW_H
