#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>
#include <string>

#include <cstdio>

float speed = 0;
float perspectiveX = 0, perspectiveY = 0, perspectiveZ = -5;
float orthoX = 0, orthoY = 0.5, orthoZ = 0;
float bridgeDegree = 0, bridgeLineUp = 0;

bool rotate = false;
bool isOrtho = false;
bool isLift = false;

int mode = 0, moveFinger = 0;

float fingerDegree = 0, handDegree = 0;

double w = 1920;
double h = 1080;
double ar = w / h; // aspect ratio

float v = -0.20;
float v1 = -0.7;

// use dedicated GPU to run
extern "C"
{
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}
extern "C"
{
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

void controls(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {
        case GLFW_KEY_ESCAPE:
            printf("v: %f\n", v);
            printf("v1: %f\n", v1);

            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        case GLFW_KEY_F1:
            mode = 0;
            if (isOrtho)
                isOrtho = false;
            else
                isOrtho = true;
            break;
        case GLFW_KEY_F2:
            mode = 1;
            break;
        case GLFW_KEY_F3:
            mode = 2;
            break;
        case GLFW_KEY_F4:
            mode = 3;
            break;
        case GLFW_KEY_SPACE:
            if (rotate)
                rotate = false;
            else
                rotate = true;
            break;
        case GLFW_KEY_RIGHT:
            perspectiveX += 0.1;
            orthoX += 0.1;
            break;
        case GLFW_KEY_LEFT:
            perspectiveX -= 0.1;
            orthoX -= 0.1;
            break;
        case GLFW_KEY_UP:
            switch (mode)
            {
            case 0:
                perspectiveY += 0.1;
                orthoY += 0.1;
                break;
            case 1:
                fingerDegree += 1;
                break;
            case 2:
                handDegree += 1;
                break;
            default:
                break;
            }
            break;
        case GLFW_KEY_DOWN:
            switch (mode)
            {
            case 0:
                perspectiveY -= 0.1;
                orthoY -= 0.1;
                break;
            case 1:
                fingerDegree -= 1;
                break;
            case 2:
                handDegree -= 1;
                break;
            default:
                break;
            }
            break;
        case GLFW_KEY_KP_8:
            perspectiveZ += 0.1;
            orthoZ += 0.1;
            break;
        case GLFW_KEY_KP_2:
            perspectiveZ -= 0.1;
            orthoZ -= 0.1;
            break;
        case GLFW_KEY_1:
            moveFinger = 0;
            break;
        case GLFW_KEY_2:
            moveFinger = 1;
            break;
        case GLFW_KEY_3:
            moveFinger = 2;
            break;
        case GLFW_KEY_4:
            moveFinger = 3;
            break;
        case GLFW_KEY_5:
            moveFinger = 4;
            break;
        default:
            break;
        }
        // else if (key == GLFW_KEY_T)
        // {
        //     v += 0.01;
        // }
        // else if (key == GLFW_KEY_G)
        // {
        //     v -= 0.01;
        // }
        // else if (key == GLFW_KEY_Y)
        // {
        //     v1 += 0.01;
        // }
        // else if (key == GLFW_KEY_H)
        // {
        //     v1 -= 0.01;
        // }
    }
}

GLFWwindow *initWindow(const int resX, const int resY)
{
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return NULL;
    }
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing

    // Open a window and create its OpenGL context
    GLFWwindow *window = glfwCreateWindow(resX, resY, "Jaeger", NULL, NULL);

    if (window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, controls);

    // Get info of GPU and supported OpenGL version
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

    glEnable(GL_DEPTH_TEST); // Depth Testing
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    return window;
}

void drawCube()
{
    GLfloat vertices[] =
        {
            -1, -1, -1, -1, -1, 1, -1, 1, 1, -1, 1, -1,
            1, -1, -1, 1, -1, 1, 1, 1, 1, 1, 1, -1,
            -1, -1, -1, -1, -1, 1, 1, -1, 1, 1, -1, -1,
            -1, 1, -1, -1, 1, 1, 1, 1, 1, 1, 1, -1,
            -1, -1, -1, -1, 1, -1, 1, 1, -1, 1, -1, -1,
            -1, -1, 1, -1, 1, 1, 1, 1, 1, 1, -1, 1};

    GLfloat colors[] =
        {
            0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0,
            1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0,
            0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0,
            0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0,
            0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0,
            0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1};

    static float alpha = 0;
    //attempt to rotate cube
    glRotatef(alpha, 0, 1, 0);

    /* We have a color array and a vertex array */
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(3, GL_FLOAT, 0, colors);

    /* Send data : 24 vertices */
    glDrawArrays(GL_QUADS, 0, 24);

    /* Cleanup states */
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    alpha += 1;
}

void drawCirle(float x1, float y1, float radius)
{
    float x2 = x1, y2 = y1;

    glColor3ub(246, 246, 246);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x1, y1);
    for (float angle = 0; angle <= 360; angle += 10)
    {
        x2 = x1 + cos(angle) * radius;
        y2 = y1 + sin(angle) * radius;
        glVertex2f(x2, y2);
    }
    glEnd();
}

void drawCylinder(double baseRadius, double topRadius, double height, int slices, int stacks)
{
    GLUquadricObj *cylinder = NULL;
    cylinder = gluNewQuadric();
    gluQuadricDrawStyle(cylinder, GLU_LINE);
    gluCylinder(cylinder, baseRadius, topRadius, height, slices, stacks);
    gluDeleteQuadric(cylinder);
}

void drawSphere(float radius, int slice, int slack)
{
    GLUquadricObj *sphere = NULL;
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_LINE);
    gluSphere(sphere, radius, slice, slack);
    gluDeleteQuadric(sphere);
}

void drawCone()
{
    glPushMatrix();
    GLUquadricObj *cylinder = NULL;
    cylinder = gluNewQuadric();
    gluQuadricDrawStyle(cylinder, GLU_FILL);
    gluCylinder(cylinder, 0.01, 0.3, 1, 20, 20);
    gluDeleteQuadric(cylinder);
    glPopMatrix();

    // glPushMatrix();
    // cylinder = gluNewQuadric();
    // glColor3ub(98, 66, 40);
    // gluQuadricDrawStyle(cylinder, GLU_LINE);
    // gluCylinder(cylinder, 0.011, 0.31, 1, 5, 5);
    // gluDeleteQuadric(cylinder);
    // glPopMatrix();
}

void drawCuboid(float size, float widthScale)
{
    glBegin(GL_LINES);
    // front top
    glColor3ub(30, 136, 229);
    glVertex3f(0, 0, size);
    glVertex3f(size * widthScale, 0, size);

    // front right
    glVertex3f(size * widthScale, 0, size);
    glVertex3f(size * widthScale, 0, 0);

    // front bottom
    glVertex3f(size * widthScale, 0, 0);
    glVertex3f(0, 0, 0);

    // front left
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, size);

    // left top
    glVertex3f(0, size, size);
    glVertex3f(0, 0, size);

    // left right
    glVertex3f(0, 0, size);
    glVertex3f(0, 0, 0);

    // left bottom
    glVertex3f(0, 0, 0);
    glVertex3f(0, size, 0);

    // left left
    glVertex3f(0, size, 0);
    glVertex3f(0, size, size);

    // right
    // right top
    glVertex3f(size * widthScale, 0, size);
    glVertex3f(size * widthScale, size, size);

    // right right
    glVertex3f(size * widthScale, size, size);
    glVertex3f(size * widthScale, size, 0);

    // right bottom
    glVertex3f(size * widthScale, size, 0);
    glVertex3f(size * widthScale, 0, 0);

    // right left
    glVertex3f(size * widthScale, 0, 0);
    glVertex3f(size * widthScale, 0, size);

    // behind top
    glVertex3f(0, size, size);
    glVertex3f(size * widthScale, size, size);

    // behind bottom
    glVertex3f(0, size, 0);
    glVertex3f(size * widthScale, size, 0);

    glEnd();
}

void drawMoon()
{
    float x = 0, y = 0, z = -15, GL_PI = 3.142, radius = 0.4;
    glPushMatrix();
    {
        // Call only once for all remaining points
        glBegin(GL_LINES);

        // All lines lie in the xy plane.
        for (float angle = 0.0f; angle <= GL_PI; angle += (GL_PI / 50.0f))
        {
            // Top half of the circle
            x = radius * sin(angle);
            y = radius * cos(angle);
            glVertex3f(x + 1, y + 2, z); // First endpoint of line

            // Bottom half of the circle
            x = radius * sin(angle + GL_PI);
            y = radius * cos(angle + GL_PI);
            glVertex3f(x + 1, y + 2, z); // Second endpoint of line
        }
        // Done drawing points
        glEnd();
    }
    glPopMatrix();
}

void drawCloud(float size)
{
    drawCirle(0, 0, size);
    drawCirle(0.2, 0, size);

    drawCirle(0.35, 0.1, size);

    drawCirle(0.2, 0.2, size);
    drawCirle(0, 0.2, size);

    drawCirle(-0.15, 0.1, size);
}

void drawClouds()
{
    glPushMatrix();
    {
        glTranslatef(-5, 1.5, -15);
        drawCloud(0.15);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-7, 1.2, -15);
        drawCloud(0.15);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-2, 1.5, -15);
        drawCloud(0.15);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(4, 1.2, -15);
        drawCloud(0.15);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(8, 1.5, -15);
        drawCloud(0.15);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(13, 1.2, -15);
        drawCloud(0.15);
    }
    glPopMatrix();
}

void drawBracers()
{
    // hand
    glPushMatrix();
    {
        glRotatef(180, 0, 1, 1);
        glTranslatef(0, 0, -0.45);
        drawCylinder(0.1, 0.2, 0.45, 20, 10);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(0.1, -0.43, 0);
        glRotatef(180, 0, 1, 1);
        glRotatef(90, 0, 1, 0);
        drawCylinder(0.05, 0.05, 0.21, 20, 10);
    }
    glPopMatrix();
    // glPushMatrix();
    // {
    //     glRotatef(180, 0, 1, 1);
    //     glTranslatef(0, 0, -0.2);
    //     drawCylinder(0.15, 0.2, 0.4, 20, 10);
    // }
    // glPopMatrix();
    // hand
    // glPushMatrix();
    // {
    //     glRotatef(180, 0, 1, 1);
    //     glTranslatef(0, 0, -0.25);
    //     drawCylinder(0.15, 0.2, 0.2, 20, 10);
    // }
    // glPopMatrix();

    // glPushMatrix();
    // {
    //     glRotatef(180, 0, 1, 1);
    //     glTranslatef(0, 0, -0.3);
    //     drawCylinder(0.15, 0.2, 0.2, 20, 10);
    // }
    // glPopMatrix();

    // glPushMatrix();
    // {
    //     glRotatef(180, 0, 1, 1);
    //     glTranslatef(0, 0, -0.35);
    //     drawCylinder(0.15, 0.2, 0.2, 20, 10);
    // }
    // glPopMatrix();
    // glPushMatrix();
    // {
    //     glRotatef(180, 0, 1, 1);
    //     glTranslatef(0, 0, -0.4);
    //     drawCylinder(0.15, 0.2, 0.2, 20, 10);
    // }
    // glPopMatrix();
    // glPushMatrix();
    // {
    //     glRotatef(180, 0, 1, 1);
    //     glTranslatef(0, 0, -0.45);
    //     drawCylinder(0.15, 0.2, 0.2, 20, 10);
    // }
    // glPopMatrix();
}

void drawFinger(float fingerSize, float fingerWidthScale)
{
    // float fingerSize = 0.3;
    // float fingerWidthScale = 2.5;

    int jointSliceStack = 10;
    float jointRadius = 0.15;

    float fingerGap = fingerSize * fingerWidthScale + jointRadius;
    float jointGap = fingerGap - (jointRadius * 0.5);

    glScalef(0.7, 0.5, 0.5);
    glPushMatrix();
    {
        drawCuboid(fingerSize, fingerWidthScale);
    }
    glPopMatrix();
    glPushMatrix();
    {
        glTranslatef(jointGap, fingerSize / 2, fingerSize / 2);
        drawSphere(jointRadius, jointSliceStack, jointSliceStack);
    }
    glPopMatrix();
    glPushMatrix();
    {
        glTranslatef(fingerGap, 0, 0);
        glRotatef(0 + fingerDegree, 0, 0, 1);
        drawCuboid(fingerSize, fingerWidthScale);
    }
    glPopMatrix();
}

void drawFingers()
{
    // // longness of finger

    // float indexRingFinger = 0;
    float middleFinger = 0.01;
    // float littleFinger = -0.02;
    // float thumb = -0.02;
    // float secondY = -0.04, secondZ = 0.2;

    glRotatef(0 + handDegree, 0, 0, 1);

    glPushMatrix();
    {
        glTranslatef(0, 0, -0.2);
        drawFinger(0.3, 2);
    }
    glPopMatrix();

    glPushMatrix();
    {
        drawFinger(0.3, 2.5);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(0, 0, 0.2);
        drawFinger(0.3, 3);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(0, 0, 0.4);
        drawFinger(0.3, 2.5);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(0, 0, 0.6);
        drawFinger(0.3, 2);
    }
    glPopMatrix();

    // // first level fingers
    // glPushMatrix();
    // {
    //     glRotatef(40, 1, 0, 0);
    //     glPushMatrix();
    //     {
    //         glTranslatef(0, -0.6 - (middleFinger * 1), 0);
    //         drawCuboid(0.04, 0, 0, 0.1 + middleFinger);
    //     }
    //     glPopMatrix();

    //     glPushMatrix();
    //     {
    //         glTranslatef(0.05, -0.6 - (indexRingFinger * 1), 0);
    //         drawCuboid(0.04, 0, 0, 0.1 + indexRingFinger);
    //     }
    //     glPopMatrix();

    //     glPushMatrix();
    //     {
    //         glTranslatef(-0.05, -0.6 - (indexRingFinger * 1), 0);
    //         drawCuboid(0.04, 0, 0, 0.1 + indexRingFinger);
    //     }
    //     glPopMatrix();

    //     glPushMatrix();
    //     {
    //         glTranslatef(-0.10, -0.6 - (littleFinger * 1), 0);
    //         drawCuboid(0.04, 0, 0, 0.1 + littleFinger);
    //     }
    //     glPopMatrix();

    //     glPushMatrix();
    //     {
    //         glRotatef(35, 0, 0, 1);
    //         glTranslatef(-0.19, -0.54 - (thumb * 1), 0);
    //         drawCuboid(0.04, 0, 0, 0.1 + thumb);
    //     }
    //     glPopMatrix();
    // }
    // glPopMatrix();

    // // second level fingers888888888888
    // glPushMatrix();
    // {
    //     glTranslatef(0, secondY, secondZ);
    //     glRotatef(20, 1, 0, 0);
    //     glPushMatrix();
    //     {
    //         glTranslatef(0, -0.73 - (middleFinger * 2), 0);
    //         drawCuboid(0.04, 0, 0, 0.08 + middleFinger);
    //     }
    //     glPopMatrix();

    //     glPushMatrix();
    //     {
    //         glTranslatef(0.05, -0.73 - (indexRingFinger * 2), 0);
    //         drawCuboid(0.04, 0, 0, 0.08 + indexRingFinger);
    //     }
    //     glPopMatrix();

    //     glPushMatrix();
    //     {
    //         glTranslatef(-0.05, -0.73 - (indexRingFinger * 2), 0);
    //         drawCuboid(0.04, 0, 0, 0.08 + indexRingFinger);
    //     }
    //     glPopMatrix();

    //     glPushMatrix();
    //     {
    //         glTranslatef(-0.10, -0.73 - (littleFinger * 2), 0);
    //         drawCuboid(0.04, 0, 0, 0.08 + littleFinger);
    //     }
    //     glPopMatrix();

    //     glPushMatrix();
    //     {
    //         glTranslatef(0.12, -0.7 - (thumb * 2), 0);
    //         glRotatef(0, 0, 0, 1);
    //         drawCuboid(0.04, 0, 0, 0.1 + thumb);
    //     }
    //     glPopMatrix();
    // }
    // glPopMatrix();

    // // third level fingers
    // glPushMatrix();
    // {
    //     glTranslatef(0, secondY * 4, secondZ * 2);
    //     glRotatef(40, 1, 0, 0);
    //     glPushMatrix();
    //     {
    //         glTranslatef(0, -0.83 - (middleFinger * 3), 0);
    //         drawCuboid(0.04, 0, 0, 0.05 + middleFinger);
    //     }
    //     glPopMatrix();

    //     glPushMatrix();
    //     {
    //         glTranslatef(0.05, -0.83 - (indexRingFinger * 3), 0);
    //         drawCuboid(0.04, 0, 0, 0.05 + indexRingFinger);
    //     }
    //     glPopMatrix();

    //     glPushMatrix();
    //     {
    //         glTranslatef(-0.05, -0.83 - (indexRingFinger * 3), 0);
    //         drawCuboid(0.04, 0, 0, 0.05 + indexRingFinger);
    //     }
    //     glPopMatrix();

    //     glPushMatrix();
    //     {
    //         glTranslatef(-0.10, -0.83 - (littleFinger * 3), 0);
    //         drawCuboid(0.04, 0, 0, 0.05 + littleFinger);
    //     }
    //     glPopMatrix();
    // }
    // glPopMatrix();

    // // first and second level finger joint
    // glPushMatrix();
    // {
    //     int jointSliceStack = 10;
    //     float jointRadius = 0.025;
    //     float jointX = 0.015;
    //     glPushMatrix();
    //     {
    //         glTranslatef(jointX, -0.6 - (middleFinger * 1), 0.01);
    //         drawSphere(jointRadius, jointSliceStack, jointSliceStack);
    //     }
    //     glPopMatrix();

    //     glPushMatrix();
    //     {
    //         glTranslatef(jointX + 0.05, -0.6 - (indexRingFinger * 1), 0.01);
    //         drawSphere(jointRadius, jointSliceStack, jointSliceStack);
    //     }
    //     glPopMatrix();

    //     glPushMatrix();
    //     {
    //         glTranslatef(jointX - 0.05, -0.6 - (indexRingFinger * 1), 0.01);
    //         drawSphere(jointRadius, jointSliceStack, jointSliceStack);
    //     }
    //     glPopMatrix();

    //     glPushMatrix();
    //     {
    //         glTranslatef(jointX - 0.10, -0.6 - (littleFinger * 1), 0.01);
    //         drawSphere(jointRadius, jointSliceStack, jointSliceStack);
    //     }
    //     glPopMatrix();

    //     glPushMatrix();
    //     {
    //         glTranslatef(jointX + 0.12, -0.55 - (thumb * 1), 0.02);
    //         glRotatef(35, 0, 0, 1);
    //         drawSphere(jointRadius, jointSliceStack, jointSliceStack);
    //     }
    //     glPopMatrix();
    // }
    // glPopMatrix();

    // // second and third level finger joint
    // glPushMatrix();
    // {
    //     float jointX = 0.015;
    //     int jointSliceStack = 10;
    //     float jointRadius = 0.025;
    //     glTranslatef(0, secondY, secondZ);
    //     glRotatef(20, 1, 0, 0);
    //     glPushMatrix();
    //     {
    //         glTranslatef(jointX, -0.73 - (middleFinger * 1), 0.01);
    //         drawSphere(jointRadius, jointSliceStack, jointSliceStack);
    //     }
    //     glPopMatrix();

    //     glPushMatrix();
    //     {
    //         glTranslatef(jointX + 0.05, -0.73 - (indexRingFinger * 1), 0.01);
    //         drawSphere(jointRadius, jointSliceStack, jointSliceStack);
    //     }
    //     glPopMatrix();

    //     glPushMatrix();
    //     {
    //         glTranslatef(jointX - 0.05, -0.73 - (indexRingFinger * 1), 0.01);
    //         drawSphere(jointRadius, jointSliceStack, jointSliceStack);
    //     }
    //     glPopMatrix();

    //     glPushMatrix();
    //     {
    //         glTranslatef(jointX - 0.10, -0.73 - (littleFinger * 1), 0.01);
    //         drawSphere(jointRadius, jointSliceStack, jointSliceStack);
    //     }
    //     glPopMatrix();
    // }
    // glPopMatrix();
}

void drawArm()
{
    // draw arm
    glPushMatrix();
    {
        glRotatef(180, 0, 1, 1);
        drawCylinder(0.15, 0.22, 1, 20, 10);
    }
    glPopMatrix();

    drawBracers();

    drawFingers();
}

void drawShoe()
{
    glPushMatrix();
    {
    glScalef(1, 0.3, 1);
    drawCuboid(0.5, 2);
    }
    glPopMatrix();
    glPushMatrix();
    {
        glTranslatef(0.1, 0.3 * 0.5, 0);
        glScalef(0.7, 0.3, 0.8);
        drawCuboid(0.5, 2);
    }
    glPopMatrix();
}

void display()
{
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glRotatef(speed, 0, 1, 0);
    glRotatef(90, 0, 1, 0);

    glPushMatrix();
    {
        // drawFingers();
        drawShoe();
    }
    glPopMatrix();

    // glPushMatrix();
    // {
    //     glRotatef(90, 0, 1, 0);
    //     glTranslatef(0.7, 0, 0);
    //     drawArm();
    // }
    // glPopMatrix();

    // glPushMatrix();
    // {
    //     glRotatef(90, 0, 1, 0);
    //     glTranslatef(-0.7, 0, 0);
    //     drawArm();
    // }
    // glPopMatrix();

    // glPushMatrix();
    // {
    //         drawBridgeCuboid(1.5, 4, 10, 1);
    // }
    // glPopMatrix();
}

int main(int argc, char **argv)
{
    GLFWwindow *window = initWindow(1920, 1080);

    if (NULL != window)
    {
        while (!glfwWindowShouldClose(window))
        {
            // Scale to window size
            GLint windowWidth, windowHeight;
            glfwGetWindowSize(window, &windowWidth, &windowHeight);
            glViewport(0, 0, windowWidth, windowHeight);

            // setup the camera and
            // toggle different view mode
            if (isOrtho)
            {
                glLoadIdentity();
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                glOrtho(-2 * ar, 2 * ar, -2, 2, -2, 2);

                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                glTranslatef(orthoX, orthoY, orthoZ);
            }
            else
            {
                glLoadIdentity();
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                gluPerspective(45, ar, 1, 2000);

                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                glTranslatef(perspectiveX, perspectiveY, perspectiveZ);
            }

            display();

            if (rotate)
                speed += 1;
            else
                speed = speed;

            if (isLift)
            {
                if (bridgeDegree < 35)
                {
                    bridgeDegree += 1;
                }
                if (bridgeLineUp < 0.26)
                {
                    bridgeLineUp += 0.008;
                }
            }
            else
            {
                if (bridgeDegree > 0)
                {
                    bridgeDegree -= 1;
                }
                if (bridgeLineUp > 0)
                {
                    bridgeLineUp -= 0.008;
                }
            }

            // Update Screen
            glfwSwapBuffers(window);

            // Check for any input, or window movement
            glfwPollEvents();
        }
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
