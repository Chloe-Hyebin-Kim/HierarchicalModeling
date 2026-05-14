#include "HierarchicalModeling.h"

void HierarchicalModeling::SetColor(float r, float g, float b)
{
    glColor3f(r, g, b);
}

double HierarchicalModeling::Rad2Deg(double radians)
{
    return radians * 180.0 / PI;
}

void HierarchicalModeling::AlignYAxisToVector(double dx, double dy, double dz) const
{
    double length = std::sqrt(dx * dx + dy * dy + dz * dz);

    if (length < 1e-6)
        return;

    double vx = dx / length;
    double vy = dy / length;
    double vz = dz / length;

    // local y-axis: e_y = (0, 1, 0)
    // target direction: v_hat = (vx, vy, vz)
    //
    // rotation axis:
    // axis = e_y x v_hat = (vz, 0, -vx)
    double axisX = vz;
    double axisY = 0.0;
    double axisZ = -vx;

    double axisLength = std::sqrt(axisX * axisX + axisY * axisY + axisZ * axisZ);

    // rotation angle:
    // theta = acos(e_y dot v_hat) = acos(vy)
    double dot = vy;
    dot = std::max(-1.0, std::min(1.0, dot));

    double angle = Rad2Deg(std::acos(dot));

    if (axisLength < 1e-6)
    {
        if (dot < 0.0)
        {
            glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
        }
        return;
    }

    glRotatef(
        static_cast<float>(angle),
        static_cast<float>(axisX / axisLength),
        static_cast<float>(axisY / axisLength),
        static_cast<float>(axisZ / axisLength)
    );
}

void HierarchicalModeling::DrawCylinderY(
    double radius,
    double height,
    float r,
    float g,
    float b,
    int segments
) const
{
    SetColor(r, g, b);

    // Side surface
    glBegin(GL_QUADS);
    for (int i = 0; i < segments; ++i)
    {
        double a0 = 2.0 * PI * i / segments;
        double a1 = 2.0 * PI * (i + 1) / segments;

        double x0 = std::cos(a0) * radius;
        double z0 = std::sin(a0) * radius;
        double x1 = std::cos(a1) * radius;
        double z1 = std::sin(a1) * radius;

        glVertex3f(static_cast<float>(x0), 0.0f, static_cast<float>(z0));
        glVertex3f(static_cast<float>(x1), 0.0f, static_cast<float>(z1));
        glVertex3f(static_cast<float>(x1), static_cast<float>(height), static_cast<float>(z1));
        glVertex3f(static_cast<float>(x0), static_cast<float>(height), static_cast<float>(z0));
    }
    glEnd();

    // Bottom cap
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.0f, 0.0f);
    for (int i = 0; i <= segments; ++i)
    {
        double a = 2.0 * PI * i / segments;

        glVertex3f(
            static_cast<float>(std::cos(a) * radius),
            0.0f,
            static_cast<float>(std::sin(a) * radius)
        );
    }
    glEnd();

    // Top cap
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, static_cast<float>(height), 0.0f);
    for (int i = 0; i <= segments; ++i)
    {
        double a = 2.0 * PI * i / segments;

        glVertex3f(
            static_cast<float>(std::cos(a) * radius),
            static_cast<float>(height),
            static_cast<float>(std::sin(a) * radius)
        );
    }
    glEnd();
}

void HierarchicalModeling::DrawCylinderBetween(
    double sx, double sy, double sz,
    double ex, double ey, double ez,
    double radius,
    float r, float g, float b,
    int segments
) const
{
    double dx = ex - sx;
    double dy = ey - sy;
    double dz = ez - sz;

    double length = std::sqrt(dx * dx + dy * dy + dz * dz);

    glPushMatrix();

    // T_link = Translate(start) * Rotate(local_y -> link_direction)
    glTranslatef(
        static_cast<float>(sx),
        static_cast<float>(sy),
        static_cast<float>(sz)
    );

    AlignYAxisToVector(dx, dy, dz);

    DrawCylinderY(radius, length, r, g, b, segments);

    glPopMatrix();
}

void HierarchicalModeling::DrawFrustumY(
    double radiusBottom,
    double radiusTop,
    double height,
    float r,
    float g,
    float b,
    int segments
) const
{
    SetColor(r, g, b);

    glBegin(GL_QUADS);
    for (int i = 0; i < segments; ++i)
    {
        double a0 = 2.0 * PI * i / segments;
        double a1 = 2.0 * PI * (i + 1) / segments;

        double bx0 = std::cos(a0) * radiusBottom;
        double bz0 = std::sin(a0) * radiusBottom;
        double bx1 = std::cos(a1) * radiusBottom;
        double bz1 = std::sin(a1) * radiusBottom;

        double tx0 = std::cos(a0) * radiusTop;
        double tz0 = std::sin(a0) * radiusTop;
        double tx1 = std::cos(a1) * radiusTop;
        double tz1 = std::sin(a1) * radiusTop;

        glVertex3f(static_cast<float>(bx0), 0.0f, static_cast<float>(bz0));
        glVertex3f(static_cast<float>(bx1), 0.0f, static_cast<float>(bz1));
        glVertex3f(static_cast<float>(tx1), static_cast<float>(height), static_cast<float>(tz1));
        glVertex3f(static_cast<float>(tx0), static_cast<float>(height), static_cast<float>(tz0));
    }
    glEnd();

    // Bottom outline
    SetColor(0.95f, 0.75f, 0.20f);
    glLineWidth(3.0f);

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; ++i)
    {
        double a = 2.0 * PI * i / segments;

        glVertex3f(
            static_cast<float>(std::cos(a) * radiusBottom),
            0.0f,
            static_cast<float>(std::sin(a) * radiusBottom)
        );
    }
    glEnd();

    glLineWidth(1.0f);
}

void HierarchicalModeling::DrawFrustumBetween(
    double sx, double sy, double sz,
    double ex, double ey, double ez,
    double radiusStart,
    double radiusEnd,
    float r, float g, float b,
    int segments
) const
{
    double dx = ex - sx;
    double dy = ey - sy;
    double dz = ez - sz;

    double length = std::sqrt(dx * dx + dy * dy + dz * dz);

    glPushMatrix();

    glTranslatef(
        static_cast<float>(sx),
        static_cast<float>(sy),
        static_cast<float>(sz)
    );

    AlignYAxisToVector(dx, dy, dz);

    DrawFrustumY(radiusStart, radiusEnd, length, r, g, b, segments);

    glPopMatrix();
}

void HierarchicalModeling::DrawSphere(
    double cx, double cy, double cz,
    double radius,
    float r,
    float g,
    float b,
    int slices,
    int stacks
) const
{
    SetColor(r, g, b);

    for (int stack = 0; stack < stacks; ++stack)
    {
        double phi0 = PI * stack / stacks;
        double phi1 = PI * (stack + 1) / stacks;

        double y0 = std::cos(phi0) * radius;
        double r0 = std::sin(phi0) * radius;

        double y1 = std::cos(phi1) * radius;
        double r1 = std::sin(phi1) * radius;

        glBegin(GL_QUAD_STRIP);
        for (int slice = 0; slice <= slices; ++slice)
        {
            double theta = 2.0 * PI * slice / slices;

            double x0 = std::cos(theta) * r0;
            double z0 = std::sin(theta) * r0;

            double x1 = std::cos(theta) * r1;
            double z1 = std::sin(theta) * r1;

            glVertex3f(
                static_cast<float>(cx + x0),
                static_cast<float>(cy + y0),
                static_cast<float>(cz + z0)
            );

            glVertex3f(
                static_cast<float>(cx + x1),
                static_cast<float>(cy + y1),
                static_cast<float>(cz + z1)
            );
        }
        glEnd();
    }
}

void HierarchicalModeling::DrawFloorGrid() const
{
    SetColor(0.35f, 0.35f, 0.35f);
    glLineWidth(1.0f);

    glBegin(GL_LINES);

    int gridSize = 5;
    double step = 0.5;

    for (int i = -gridSize * 2; i <= gridSize * 2; ++i)
    {
        double p = i * step;

        glVertex3f(static_cast<float>(-gridSize), 0.0f, static_cast<float>(p));
        glVertex3f(static_cast<float>(gridSize), 0.0f, static_cast<float>(p));

        glVertex3f(static_cast<float>(p), 0.0f, static_cast<float>(-gridSize));
        glVertex3f(static_cast<float>(p), 0.0f, static_cast<float>(gridSize));
    }

    glEnd();
}

void HierarchicalModeling::DrawStandLamp() const
{
    // Base
    double baseRadius = 0.65;
    double baseHeight = 0.25;

    DrawCylinderY(baseRadius, baseHeight, 0.30f, 0.30f, 0.30f);

    // p0 = base_top
    double p0x = 0.0;
    double p0y = baseHeight;
    double p0z = 0.0;

    DrawSphere(p0x, p0y, p0z, 0.10, 0.85f, 0.85f, 0.85f);

    // p1 = neck_end
    double p1x = 0.0;
    double p1y = 1.10;
    double p1z = 0.0;

    // p2 = arm_end
    double p2x = 1.85;
    double p2y = 2.25;
    double p2z = 0.0;

    // Link 1: p0 -> p1
    DrawCylinderBetween(
        p0x, p0y, p0z,
        p1x, p1y, p1z,
        0.07,
        0.20f, 0.55f, 0.90f
    );

    DrawSphere(p1x, p1y, p1z, 0.12, 0.85f, 0.85f, 0.85f);

    // Link 2: p1 -> p2
    DrawCylinderBetween(
        p1x, p1y, p1z,
        p2x, p2y, p2z,
        0.07,
        0.20f, 0.55f, 0.90f
    );

    DrawSphere(p2x, p2y, p2z, 0.12, 0.85f, 0.85f, 0.85f);

    // p3 = head_end
    double p3x = 2.30;
    double p3y = 1.45;
    double p3z = 0.0;

    // Lamp head: p2 -> p3
    DrawFrustumBetween(
        p2x, p2y, p2z,
        p3x, p3y, p3z,
        0.18,
        0.48,
        0.95f, 0.80f, 0.25f
    );
}

void HierarchicalModeling::Render() const
{
    DrawFloorGrid();
    DrawStandLamp();
}