#pragma once
#include <cmath>
#include <algorithm>
#include <GL/freeglut.h>

using namespace std;

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

    void AlignYAxisToVector(double dx, double dy, double dz) const;
    
    void DrawBoxY(double width,double height,double depth, float r, float g,float b) const;
    void DrawCylinderY(double radius,double height,float r,float g,float b,int segments = 40) const;
    void DrawFrustumY(double radiusBottom, double radiusTop, double height, float r, float g, float b, int segments = 40) const;
    void DrawSphere(double cx, double cy, double cz, double radius, float r, float g, float b, int slices = 24, int stacks = 12) const;

    void DrawFloorGrid() const;

    void DrawStandLamp() const;

    void DrawCylinderBetween(double sx, double sy, double sz,double ex, double ey, double ez,double radius,float r, float g, float b,int segments = 40) const;
    void DrawFrustumBetween(double sx, double sy, double sz,double ex, double ey, double ez,double radiusStart,double radiusEnd,float r, float g, float b,int segments = 40) const;
};
