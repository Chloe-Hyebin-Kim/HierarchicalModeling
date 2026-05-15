#include "HierarchicalModeling.h"

void HierarchicalModeling::SetColor(float r, float g, float b)
{
    glColor3f(r, g, b);
}

double HierarchicalModeling::Rad2Deg(double radians)
{
    return radians * 180.0 / PI;
}

double HierarchicalModeling::Lerp(double a, double b, double t)
{
    // t = 0이면 a
    // t = 1이면 b
    // 0 < t < 1이면 a와 b 사이 값
    return a + (b - a) * t;
}

void HierarchicalModeling::AlignYAxisToVector(double dx, double dy, double dz) const
{
    double f64Length = sqrt(dx * dx + dy * dy + dz * dz);

    if (f64Length < 1e-6)
        return;

    double vx = dx / f64Length;
    double vy = dy / f64Length;
    double vz = dz / f64Length;

    // 기준축 ==(0, 1, 0)
    // 회전방향  (vx, vy, vz)
    // 회전 축 == (0, 1, 0) 외적 (vx, vy, vz) = (vz, 0, -vx)
    double axisX = vz;
    double axisY = 0.0;
    double axisZ = -vx;

    double f64AxisLength = sqrt(axisX * axisX + axisY * axisY + axisZ * axisZ);

    // rotation angle:
    // f64Theta = acos(e_y dot v_hat) = acos(vy)
    double dot = vy;
    dot = max(-1.0, min(1.0, dot));

    double angle = Rad2Deg(acos(dot));

    if (f64AxisLength < 1e-6)// y축과 거의 평행한 경우
    {
        if (dot < 0.0)// 반대 방향이므로 x축 기준 180도 회전.
        {
            glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
        }
        return;
    }

    // axis를 단위 벡터로 정규화한 뒤 축각회전 적용
    glRotatef((float)(angle),(float)(axisX / f64AxisLength),(float)(axisY / f64AxisLength),(float)(axisZ / f64AxisLength));
}

void HierarchicalModeling::DrawBoxY(double width,double height,double depth,float r,float g,float b) const
{
    SetColor(r, g, b);

    double x = width / 2.0;
    double z = depth / 2.0;

    glBegin(GL_QUADS);

    // Front face
    glVertex3f((float)(-x), 0.0f,(float)(z));
    glVertex3f((float)(x), 0.0f,(float)(z));
    glVertex3f((float)(x),(float)(height),(float)(z));
    glVertex3f((float)(-x),(float)(height),(float)(z));

    // Back face
    glVertex3f((float)(x), 0.0f,(float)(-z));
    glVertex3f((float)(-x), 0.0f,(float)(-z));
    glVertex3f((float)(-x),(float)(height),(float)(-z));
    glVertex3f((float)(x),(float)(height),(float)(-z));

    // Left face
    glVertex3f((float)(-x), 0.0f,(float)(-z));
    glVertex3f((float)(-x), 0.0f,(float)(z));
    glVertex3f((float)(-x),(float)(height),(float)(z));
    glVertex3f((float)(-x),(float)(height),(float)(-z));

    // Right face
    glVertex3f((float)(x), 0.0f,(float)(z));
    glVertex3f((float)(x), 0.0f,(float)(-z));
    glVertex3f((float)(x),(float)(height),(float)(-z));
    glVertex3f((float)(x),(float)(height),(float)(z));

    // Bottom face
    glVertex3f((float)(-x), 0.0f,(float)(-z));
    glVertex3f((float)(x), 0.0f,(float)(-z));
    glVertex3f((float)(x), 0.0f,(float)(z));
    glVertex3f((float)(-x), 0.0f,(float)(z));

    // Top face
    glVertex3f((float)(-x),(float)(height),(float)(z));
    glVertex3f((float)(x),(float)(height),(float)(z));
    glVertex3f((float)(x),(float)(height),(float)(-z));
    glVertex3f((float)(-x),(float)(height),(float)(-z));

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

        double x0 = cos(a0) * radius;
        double z0 = sin(a0) * radius;
        double x1 = cos(a1) * radius;
        double z1 = sin(a1) * radius;

        glVertex3f((float)(x0), 0.0f,(float)(z0));
        glVertex3f((float)(x1), 0.0f,(float)(z1));
        glVertex3f((float)(x1),(float)(height),(float)(z1));
        glVertex3f((float)(x0),(float)(height),(float)(z0));
    }
    glEnd();

    // Bottom cap
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.0f, 0.0f);
    for (int i = 0; i <= segments; ++i)
    {
        double a = 2.0 * PI * i / segments;
        glVertex3f((float)(cos(a) * radius),0.0f,(float)(sin(a) * radius));
    }
    glEnd();

    // Top cap
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f,(float)(height), 0.0f);
    for (int i = 0; i <= segments; ++i)
    {
        double a = 2.0 * PI * i / segments;
        glVertex3f((float)(cos(a) * radius),(float)(height),(float)(sin(a) * radius));
    }
    glEnd();
}

void HierarchicalModeling::DrawCylinderBetween( double sx, double sy, double sz,double ex, double ey, double ez, double radius, float r, float g, float b, int segments) const
{
    double dx = ex - sx;
    double dy = ey - sy;
    double dz = ez - sz;

    double f64Length = sqrt(dx * dx + dy * dy + dz * dz);

    glPushMatrix();

    // T_link = Translate(start) * Rotate(local_y -> link_direction)
    glTranslatef((float)(sx),(float)(sy),(float)(sz));
    AlignYAxisToVector(dx, dy, dz);
    DrawCylinderY(radius, f64Length, r, g, b, segments);
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

        double bx0 = cos(a0) * radiusBottom;
        double bz0 = sin(a0) * radiusBottom;
        double bx1 = cos(a1) * radiusBottom;
        double bz1 = sin(a1) * radiusBottom;

        double tx0 = cos(a0) * radiusTop;
        double tz0 = sin(a0) * radiusTop;
        double tx1 = cos(a1) * radiusTop;
        double tz1 = sin(a1) * radiusTop;

        glVertex3f((float)(bx0), 0.0f,(float)(bz0));
        glVertex3f((float)(bx1), 0.0f,(float)(bz1));
        glVertex3f((float)(tx1),(float)(height),(float)(tz1));
        glVertex3f((float)(tx0),(float)(height),(float)(tz0));
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
           (float)(cos(a) * radiusBottom),
            0.0f,
           (float)(sin(a) * radiusBottom)
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

    double f64Length = sqrt(dx * dx + dy * dy + dz * dz);

    glPushMatrix();
    glTranslatef((float)(sx),(float)(sy),(float)(sz));
    AlignYAxisToVector(dx, dy, dz);
    DrawFrustumY(radiusStart, radiusEnd, f64Length, r, g, b, segments);
    glPopMatrix();
}

void HierarchicalModeling::DrawSphere(double cx, double cy, double cz,double radius,float r,float g,float b,int slices,int stacks) const
{
    SetColor(r, g, b);

    for (int stack = 0; stack < stacks; ++stack)
    {
        double phi0 = PI * stack / stacks;
        double phi1 = PI * (stack + 1) / stacks;

        double y0 = cos(phi0) * radius;
        double r0 = sin(phi0) * radius;

        double y1 = cos(phi1) * radius;
        double r1 = sin(phi1) * radius;

        glBegin(GL_QUAD_STRIP);
        for (int slice = 0; slice <= slices; ++slice)
        {
            double f64Theta = 2.0 * PI * slice / slices;

            double x0 = cos(f64Theta) * r0;
            double z0 = sin(f64Theta) * r0;

            double x1 = cos(f64Theta) * r1;
            double z1 = sin(f64Theta) * r1;

            glVertex3f(
               (float)(cx + x0),
               (float)(cy + y0),
               (float)(cz + z0)
            );

            glVertex3f(
               (float)(cx + x1),
               (float)(cy + y1),
               (float)(cz + z1)
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

        glVertex3f((float)(-gridSize), 0.0f,(float)(p));
        glVertex3f((float)(gridSize), 0.0f,(float)(p));

        glVertex3f((float)(p), 0.0f,(float)(-gridSize));
        glVertex3f((float)(p), 0.0f,(float)(gridSize));
    }

    glEnd();
}

void HierarchicalModeling::DrawStandLamp() const
{
// Pose 0: base=-35, arm=-75, head=-115
// Pose 1: base=-10, arm=-55, head=-95
// Pose 2: base= 20, arm=-35, head=-75
// Pose 3: base= 35, arm=-55, head=-95
        // 인덱스 같으면 같은 순간 
    //const double f64BaseYawDegArr[4] = {-35.0, -10.0, 20.0, 35.0};
    //const double f64ArmRollDegArr[4] = {-75.0, -55.0, -35.0, -55.0};
    //const double f64HeadRollDegArr[4] = {-115.0, -95.0, -75.0, -95.0};
    const double holdTime = 0.8;

    //움직임 부드럽게 변경
    const double f64BaseYawDegArr[4] = { -1.0, 2.0, 5.0, 2.0}; // y축
    const double f64ArmRollDegArr[4] = { -18.0, -25.0, -32.0, -20.0 }; // z축
    const double f64HeadRollDegArr[4] = { -98.0, -94.0, -90.0, -93.0 }; // z축

    // 흐름 계산
    double keyframePosition = m_f64TimeValue / holdTime;

    // 현재 위치
    int currentStep = (int)(floor(keyframePosition)) % 4;

    // 다음 위히
    int nextStep = (currentStep + 1) % 4;

    // 진행률
    double alpha = keyframePosition - floor(keyframePosition);

    //각 관절 보간   theta(t) = (1 - alpha) * theta_current + alpha * theta_next
    double f64BaseYawAngle = Lerp(f64BaseYawDegArr[currentStep],f64BaseYawDegArr[nextStep],alpha);
    double f64ArmRollAngle = Lerp(f64ArmRollDegArr[currentStep],f64ArmRollDegArr[nextStep],alpha);
    double f64HeadRollAngle = Lerp(f64HeadRollDegArr[currentStep],f64HeadRollDegArr[nextStep],alpha);

    //const double holdTime = 0.65;    // holdTime초마다 전환
    //int step = (int)(m_f64TimeValue / holdTime) % 4;
    //double f64BaseYawAngle = f64BaseYawDegArr[step];
    //double f64ArmRollAngle = f64ArmRollDegArr[step];
    //double f64HeadRollAngle = f64HeadRollDegArr[step];

    // Link lengths
    const double f64NeckLength = 0.85;
    const double f64LowerArmLength = 1.85;
    const double f64LampHeadLength = 0.75;
    
    // Base size
    const double f64BaseWidth = 1.2;
    const double f64BaseDepth = 1.2;
    const double f64BaseHeight = 0.25;

    glPushMatrix();

    //DrawCylinderY(baseRadius, baseHeight, 0.30f, 0.30f, 0.30f);
    DrawBoxY(f64BaseWidth, f64BaseHeight, f64BaseDepth, 0.30f, 0.30f, 0.30f);


    //  Base 윗면 중심==neck link의 시작 관절
    glTranslatef(0.0f,(float)(f64BaseHeight), 0.0f);
    // p0 = base_top
    //DrawSphere(0.0, baseHeight, 0.0, 0.10, 0.85f, 0.85f, 0.85f);
    DrawSphere(0.0, 0.0, 0.0,0.10,0.85f, 0.85f, 0.85f);
    
    glRotatef((float)(f64BaseYawAngle),0.0f, 1.0f, 0.0f);
   
    // Link 1: p0 -> p1
    //DrawCylinderBetween(0.0, baseHeight, 0.0, 0.0, 1.10, 0.0, 0.07, 0.20f, 0.55f, 0.90f);
    DrawCylinderY(0.07,f64NeckLength,0.20f, 0.55f, 0.90f);
    glTranslatef(0.0f,(float)(f64NeckLength), 0.0f);

  
    DrawSphere(0.0f, 0.0f, 0.0f, 0.12, 0.85f, 0.85f, 0.85f);
    glRotatef((float)(f64ArmRollAngle), 0.0f, 0.0f, 1.0f);
   
    DrawCylinderY( 0.07,f64LowerArmLength,0.20f, 0.55f, 0.90f);
    glTranslatef(0.0f,(float)(f64LowerArmLength), 0.0f);

    DrawSphere(0.0f, 0.0f, 0.0f, 0.12, 0.85f, 0.85f, 0.85f);
    glRotatef((float)(f64HeadRollAngle),0.0f, 0.0f, 1.0f);
    DrawFrustumY(0.18,0.48,f64LampHeadLength,0.95f, 0.80f, 0.25f);

    glPopMatrix();
}

void HierarchicalModeling::Render() const
{
    DrawFloorGrid();
    DrawStandLamp();
}

void HierarchicalModeling::Update(double dt)
{
    m_f64TimeValue += dt;
}
