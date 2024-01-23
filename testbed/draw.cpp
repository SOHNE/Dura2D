#include "draw.h"

#include "raylib.h"

#include <cmath>

draw g_draw;

draw::draw()
{

}

draw::~draw()
{

}

void
draw::DrawPolygon(const d2Vec2 *vertices, int32 vertexCount, const float &angle, const d2Color &color)
{
    const Color c = convertToColor(color);

    for (int i = 0; i < vertexCount; i++) {
        int j = (i + 1) % vertexCount;
        ::DrawLineV(Vector2{vertices[i].x, vertices[i].y}, Vector2{vertices[j].x, vertices[j].y}, c);
    }
}

void
draw::DrawSolidPolygon(const d2Vec2 *vertices, int32 vertexCount, const float &angle, const d2Color &color)
{
    const Color fillColor = convertToColor(color * 0.5f);
    const Color borderColor = convertToColor(color);

    // Draw the edges of the polygon
    for (int i = 0; i < vertexCount; ++i)
    {
        ::DrawLineV(Vector2{vertices[i].x, vertices[i].y},
                  Vector2{vertices[(i + 1) % vertexCount].x, vertices[(i + 1) % vertexCount].y},
                  borderColor);
    }

    // Draw the interior of the polygon. split into triangles
    for (int i = 1; i < vertexCount - 1; ++i)
    {
        ::DrawTriangle(
                (Vector2) {vertices[0].x, vertices[0].y},
                (Vector2) {vertices[i + 1].x, vertices[i + 1].y},
                (Vector2) {vertices[i].x, vertices[i].y},
                fillColor);

        // draw triangle lines
        /*::DrawLineV(Vector2{vertices[0].x, vertices[0].y},
                    Vector2{vertices[i + 1].x, vertices[i + 1].y},
                    borderColor);*/
    }
}


void
draw::DrawCircle(const d2Vec2 &center, float radius, const float &angle, const d2Color &color)
{
    ::DrawCircleV(Vector2{center.x, center.y}, radius, convertToColor(color));
}

void
draw::DrawSolidCircle(const d2Vec2 &center, float radius, const float &angle, const d2Color &color)
{
    ::DrawCircle(center.x, center.y, radius, convertToColor(color * 0.5f));
    ::DrawCircleLines(center.x, center.y, radius, convertToColor(color));
}

void
draw::DrawTransform(const d2Vec2 &position, float angle)
{
    // Draw the center
    ::DrawCircleV((Vector2) {position.x, position.y}, 2.5f, YELLOW);

    // Draw the angle (X and Y components)
    Vector2 angleVector = (Vector2) {std::cos(angle), std::sin(angle)};
    ::DrawLineV((Vector2) {position.x, position.y}, (Vector2) {position.x + std::cos(angle) * 25.0f, position.y + std::sin(angle) * 25.0f}, RED);
    ::DrawLineV((Vector2) {position.x, position.y}, (Vector2) {position.x + angleVector.y * 25.0f, position.y - angleVector.x * 25.0f}, GREEN);
}

void
draw::DrawSegment(const d2Vec2 &p1, const d2Vec2 &p2, const d2Color &color)
{
    ::DrawLineV((Vector2) {p1.x, p1.y}, (Vector2) {p2.x, p2.y}, convertToColor(color));
}

::Color
draw::convertToColor(const d2Color &color)
{
    d2Color c = color * 255;
    return {
            static_cast<uint8>(c.r),
            static_cast<uint8>(c.g),
            static_cast<uint8>(c.b),
            static_cast<uint8>(c.a)
    };
}
