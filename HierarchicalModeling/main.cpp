#include <cmath>
#include <GL/freeglut.h>

#include "HierarchicalModeling.h"

constexpr int WINDOW_WIDTH = 900;
constexpr int WINDOW_HEIGHT = 700;

HierarchicalModeling g_model;


void Timer(int value);

void Perspective(double fovY, double aspect, double zNear, double zFar)
{
    constexpr double PI = 3.14159265358979323846;

    double top = zNear * std::tan((fovY * PI / 180.0) / 2.0);
    double bottom = -top;
    double right = top * aspect;
    double left = -right;

    glFrustum(left, right, bottom, top, zNear, zFar);
}

void Reshape(int width, int height)
{
    if (height == 0)
        height = 1;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    double aspect = static_cast<double>(width) / static_cast<double>(height);

    Perspective(
        45.0,
        aspect,
        0.1,
        100.0
    );

    glMatrixMode(GL_MODELVIEW);
}

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Camera view
    glTranslatef(0.0f, -1.2f, -7.0f);
    glRotatef(25.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(-35.0f, 0.0f, 1.0f, 0.0f);

    g_model.Render();

    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Assignment1_HierarchicalModel - 3D Stand Lamp");

    glClearColor(0.06f, 0.07f, 0.09f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);

    glutTimerFunc(16, Timer, 0);

    glutMainLoop();

    return 0;
}

void Timer(int value)
{
    constexpr double dt = 1.0 / 60.0;

    g_model.Update(dt);

    glutPostRedisplay();

    glutTimerFunc(16, Timer, 0);
}