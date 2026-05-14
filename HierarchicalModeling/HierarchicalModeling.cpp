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

    // 기준축 ==(0, 1, 0)
    // 회전방향  (vx, vy, vz)
    // 회전 축 == (0, 1, 0) 외적 (vx, vy, vz) = (vz, 0, -vx)
    double axisX = vz;
    double axisY = 0.0;
    double axisZ = -vx;

    double axisLength = std::sqrt(axisX * axisX + axisY * axisY + axisZ * axisZ);

    // rotation angle:
    // theta = acos(e_y dot v_hat) = acos(vy)
    double dot = vy;
    dot = std::max(-1.0, std::min(1.0, dot));

    double angle = Rad2Deg(std::acos(dot));

    if (axisLength < 1e-6)// y축과 거의 평행한 경우
    {
        if (dot < 0.0)// 반대 방향이므로 x축 기준 180도 회전.
        {
            glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
        }
        return;
    }

    // axis를 단위 벡터로 정규화한 뒤 축각회전 적용
    glRotatef(static_cast<float>(angle),static_cast<float>(axisX / axisLength),static_cast<float>(axisY / axisLength),static_cast<float>(axisZ / axisLength));
}

void HierarchicalModeling::DrawBoxY(double width,double height,double depth,float r,float g,float b) const
{
    SetColor(r, g, b);

    double x = width / 2.0;
    double z = depth / 2.0;

    glBegin(GL_QUADS);

    // Front face
    glVertex3f(static_cast<float>(-x), 0.0f, static_cast<float>(z));
    glVertex3f(static_cast<float>(x), 0.0f, static_cast<float>(z));
    glVertex3f(static_cast<float>(x), static_cast<float>(height), static_cast<float>(z));
    glVertex3f(static_cast<float>(-x), static_cast<float>(height), static_cast<float>(z));

    // Back face
    glVertex3f(static_cast<float>(x), 0.0f, static_cast<float>(-z));
    glVertex3f(static_cast<float>(-x), 0.0f, static_cast<float>(-z));
    glVertex3f(static_cast<float>(-x), static_cast<float>(height), static_cast<float>(-z));
    glVertex3f(static_cast<float>(x), static_cast<float>(height), static_cast<float>(-z));

    // Left face
    glVertex3f(static_cast<float>(-x), 0.0f, static_cast<float>(-z));
    glVertex3f(static_cast<float>(-x), 0.0f, static_cast<float>(z));
    glVertex3f(static_cast<float>(-x), static_cast<float>(height), static_cast<float>(z));
    glVertex3f(static_cast<float>(-x), static_cast<float>(height), static_cast<float>(-z));

    // Right face
    glVertex3f(static_cast<float>(x), 0.0f, static_cast<float>(z));
    glVertex3f(static_cast<float>(x), 0.0f, static_cast<float>(-z));
    glVertex3f(static_cast<float>(x), static_cast<float>(height), static_cast<float>(-z));
    glVertex3f(static_cast<float>(x), static_cast<float>(height), static_cast<float>(z));

    // Bottom face
    glVertex3f(static_cast<float>(-x), 0.0f, static_cast<float>(-z));
    glVertex3f(static_cast<float>(x), 0.0f, static_cast<float>(-z));
    glVertex3f(static_cast<float>(x), 0.0f, static_cast<float>(z));
    glVertex3f(static_cast<float>(-x), 0.0f, static_cast<float>(z));

    // Top face
    glVertex3f(static_cast<float>(-x), static_cast<float>(height), static_cast<float>(z));
    glVertex3f(static_cast<float>(x), static_cast<float>(height), static_cast<float>(z));
    glVertex3f(static_cast<float>(x), static_cast<float>(height), static_cast<float>(-z));
    glVertex3f(static_cast<float>(-x), static_cast<float>(height), static_cast<float>(-z));

    glEnd();
}

void HierarchicalModeling::DrawCylinderY(double radius,double height,float r,float g,float b,int segments) const
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
        glVertex3f(static_cast<float>(std::cos(a) * radius),0.0f,static_cast<float>(std::sin(a) * radius));
    }
    glEnd();

    // Top cap
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, static_cast<float>(height), 0.0f);
    for (int i = 0; i <= segments; ++i)
    {
        double a = 2.0 * PI * i / segments;
        glVertex3f(static_cast<float>(std::cos(a) * radius), static_cast<float>(height),static_cast<float>(std::sin(a) * radius));
    }
    glEnd();
}

void HierarchicalModeling::DrawCylinderBetween( double sx, double sy, double sz,double ex, double ey, double ez, double radius, float r, float g, float b, int segments) const
{
    double dx = ex - sx;
    double dy = ey - sy;
    double dz = ez - sz;

    double length = std::sqrt(dx * dx + dy * dy + dz * dz);

    glPushMatrix();

    // T_link = Translate(start) * Rotate(local_y -> link_direction)
    glTranslatef(static_cast<float>(sx),static_cast<float>(sy),static_cast<float>(sz));
    AlignYAxisToVector(dx, dy, dz);
    DrawCylinderY(radius, length, r, g, b, segments);
    glPopMatrix();
}

void HierarchicalModeling::DrawFrustumY(double radiusBottom,double radiusTop,double height,float r,float g,float b,int segments) const
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

void HierarchicalModeling::DrawFrustumBetween(double sx, double sy, double sz,double ex, double ey, double ez, double radiusStart,double radiusEnd,float r, float g, float b,int segments) const
{
    double dx = ex - sx;
    double dy = ey - sy;
    double dz = ez - sz;

    double length = std::sqrt(dx * dx + dy * dy + dz * dz);

    glPushMatrix();
    glTranslatef(static_cast<float>(sx), static_cast<float>(sy), static_cast<float>(sz));
    AlignYAxisToVector(dx, dy, dz);
    DrawFrustumY(radiusStart, radiusEnd, length, r, g, b, segments);
    glPopMatrix();
}

void HierarchicalModeling::DrawSphere(double cx, double cy, double cz,double radius,float r,float g,float b,int slices,int stacks) const
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
    // step = int(time / holdTime) % 4
    const double holdTime = 0.65;    // holdTime초마다 전환
    int step = static_cast<int>(m_timeValue / holdTime) % 4;

    const double baseYawKeyframes[4] = {-35.0, -10.0, 20.0, 35.0};
    const double lowerArmKeyframes[4] = {-75.0, -55.0, -35.0, -55.0};
    const double lampHeadKeyframes[4] = {-115.0, -95.0, -75.0, -95.0};

    double baseYawAngle = baseYawKeyframes[step];
    double lowerArmAngle = lowerArmKeyframes[step];
    double lampHeadAngle = lampHeadKeyframes[step];

    // Link lengths
    const double neckLength = 0.85;
    const double lowerArmLength = 1.85;
    const double lampHeadLength = 0.75;
    
    // Base size
    const double baseWidth = 1.2;
    const double baseDepth = 1.2;
    const double baseHeight = 0.25;

    glPushMatrix();

    //DrawCylinderY(baseRadius, baseHeight, 0.30f, 0.30f, 0.30f);
    DrawBoxY(baseWidth,baseHeight, baseDepth,0.30f, 0.30f, 0.30f);


    //  Base 윗면 중심==neck link의 시작 관절
    glTranslatef(0.0f, static_cast<float>(baseHeight), 0.0f);
    // p0 = base_top
    //DrawSphere(0.0, baseHeight, 0.0, 0.10, 0.85f, 0.85f, 0.85f);
    DrawSphere(0.0, 0.0, 0.0,0.10,0.85f, 0.85f, 0.85f);
    
    glRotatef(static_cast<float>(baseYawAngle),0.0f, 1.0f, 0.0f);
   
    // Link 1: p0 -> p1
    //DrawCylinderBetween(0.0, baseHeight, 0.0, 0.0, 1.10, 0.0, 0.07, 0.20f, 0.55f, 0.90f);
    DrawCylinderY(0.07,neckLength,0.20f, 0.55f, 0.90f);
    glTranslatef(0.0f, static_cast<float>(neckLength), 0.0f);

  
    DrawSphere(0.0f, 0.0f, 0.0f, 0.12, 0.85f, 0.85f, 0.85f);
    glRotatef( static_cast<float>(lowerArmAngle), 0.0f, 0.0f, 1.0f);
   
    DrawCylinderY( 0.07,lowerArmLength,0.20f, 0.55f, 0.90f);
    glTranslatef(0.0f, static_cast<float>(lowerArmLength), 0.0f);

    DrawSphere(0.0f, 0.0f, 0.0f, 0.12, 0.85f, 0.85f, 0.85f);
    glRotatef(static_cast<float>(lampHeadAngle),0.0f, 0.0f, 1.0f);
    DrawFrustumY(0.18,0.48,lampHeadLength,0.95f, 0.80f, 0.25f);

    glPopMatrix();
}

void HierarchicalModeling::Render() const
{
    DrawFloorGrid();
    DrawStandLamp();
}

void HierarchicalModeling::Update(double dt)
{
    m_timeValue += dt;
}
