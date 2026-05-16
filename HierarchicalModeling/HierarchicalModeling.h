#pragma once
#include <cmath>
#include <algorithm>
#include <GL/freeglut.h>

#define MOTION_CNT 5
#define LIGHT_GRAY 0.85f, 0.85f, 0.85f
#define DARK_GRAY 0.30f, 0.30f, 0.30f
#define SOFTBLUE 0.20f, 0.55f, 0.90f
#define GOLDENYELLOW 0.95f, 0.80f, 0.25f


using namespace std;

struct F32RGB
{
    float r, g, b;
	F32RGB() : r(0.0f), g(0.0f), b(0.0f) {}
    F32RGB(float r, float g, float b) : r(r), g(g), b(b) {}

};

class HierarchicalModeling
{
public:
    HierarchicalModeling() = default;

    void Update(double dt);
    void Render() const;

private:
    static constexpr double PI = 3.14159265358979323846;
    double m_f64TimeValue = 0.0;

private:
    static void SetColor(float r, float g, float b);
    static double Rad2Deg(double radians);
    static double Lerp(double a, double b, double t);

    void AlignYAxisToVector(double dx, double dy, double dz) const;
    
    void DrawBoxY(double width,double height,double depth, F32RGB rgb) const;
    void DrawCylinderY(double radius,double height,F32RGB rgb, int segments = 40) const;
    void DrawFrustumY(double radiusBottom, double radiusTop, double height, F32RGB rgb, int segments = 40) const;
    void DrawSphere(double cx, double cy, double cz, double radius, F32RGB rgb, int slices = 24, int stacks = 12) const;

    void DrawFloorGrid() const;

    void DrawStandLamp() const;

    void DrawCylinderBetween(double sx, double sy, double sz,double ex, double ey, double ez,double radius,F32RGB rgb,int segments = 40) const;
    void DrawFrustumBetween(double sx, double sy, double sz,double ex, double ey, double ez,double radiusStart,double radiusEnd,F32RGB rgb,int segments = 40) const;
};
