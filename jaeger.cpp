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
bool rotateLeft = false;
bool rotateRight = false;

bool isOrtho = false;
bool isLift = false;

int mode = 0, moveFinger = 0;

float fingerDegree = 0, handDegree = 0;
float robotHeight = 0.5;
float calfDegree = 0, thighDegree = 0;

double w = 1920;
double h = 1080;
double ar = w / h; // aspect ratio

float v = 0;
float v1 = 0;
float v2 = 5.73;
float v3 = 0;
float v4 = 0.31;
float v5 = 0.16;
float v6 = 0.88;
float v7 = 2.64;

//
int gluDrawStyles[4] = {100012, 100010, 100011, 100011};
int glDrawStyles[4] = {0x0007, 0x0000, 0x0002, 0x0001};
int glewDrawStyles[4] = {0x0009, 0x0000, 0x0002, 0x0001};

int selectedGluDrawStyles = 100011;
int selectedGlDrawStyles = 0x0002;
int selectedGlewDrawStyles = 0x0002;

int stylesNo = 0;

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
            printf("v: %f\tv1: %f\tv2: %f\tv3: %f\n", v, v1, v2, v3);
            printf("v4: %f\tv5: %f\tv6: %f\tv7: %f\n", v4, v5, v6, v7);

            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        case GLFW_KEY_GRAVE_ACCENT:
            stylesNo += 1;
            if (stylesNo % 4 == 0)
            {
                stylesNo = 0;
            }
            selectedGluDrawStyles = gluDrawStyles[stylesNo];
            selectedGlDrawStyles = glDrawStyles[stylesNo];
            selectedGlewDrawStyles = glewDrawStyles[stylesNo];
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
        case GLFW_KEY_F5:
            mode = 4;
            break;
        case GLFW_KEY_F6:
            mode = 5;
            break;

        case GLFW_KEY_SPACE:
            if (rotate)
                rotate = false;
            else
                rotate = true;
            break;
        case GLFW_KEY_COMMA:
            speed -= 1;
            break;
        case GLFW_KEY_PERIOD:
            speed += 1;
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
            case 3:
                robotHeight += 0.1;
                break;
            case 4:
                calfDegree += 1;
                break;
            case 5:
                thighDegree += 1;
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
            case 3:
                robotHeight -= 0.1;
                break;
            case 4:
                calfDegree -= 1;
                break;
            case 5:
                thighDegree -= 1;
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
        case GLFW_KEY_T:
            v += 0.01;
            break;
        case GLFW_KEY_G:
            v -= 0.01;
            break;
        case GLFW_KEY_Y:
            v1 += 0.01;
            break;
        case GLFW_KEY_H:
            v1 -= 0.01;
            break;
        case GLFW_KEY_U:
            v2 += 0.01;
            break;
        case GLFW_KEY_J:
            v2 -= 0.01;
            break;
        case GLFW_KEY_I:
            v3 += 0.01;
            break;
        case GLFW_KEY_K:
            v3 -= 0.01;
            break;
        case GLFW_KEY_O:
            v4 += 0.01;
            break;
        case GLFW_KEY_L:
            v4 -= 0.01;
            break;
        case GLFW_KEY_P:
            v5 += 0.01;
            break;
        case GLFW_KEY_SEMICOLON:
            v5 -= 0.01;
            break;
        case GLFW_KEY_LEFT_BRACKET:
            v6 += 0.01;
            break;
        case GLFW_KEY_APOSTROPHE:
            v6 -= 0.01;
            break;
        case GLFW_KEY_RIGHT_BRACKET:
            v7 += 0.01;
            break;
        case GLFW_KEY_BACKSLASH:
            v7 -= 0.01;
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
    gluQuadricDrawStyle(cylinder, selectedGluDrawStyles);
    gluCylinder(cylinder, baseRadius, topRadius, height, slices, stacks);
    gluDeleteQuadric(cylinder);
}

void drawSphere(float radius, int slice, int slack)
{
    GLUquadricObj *sphere = NULL;
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, selectedGluDrawStyles);
    gluSphere(sphere, radius, slice, slack);
    gluDeleteQuadric(sphere);
}

void drawCone()
{
    glPushMatrix();
    GLUquadricObj *cylinder = NULL;
    cylinder = gluNewQuadric();
    gluQuadricDrawStyle(cylinder, selectedGluDrawStyles);
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
    glBegin(selectedGlDrawStyles);
    // front
    glColor3ub(30, 136, 229);
    glVertex3f(0, 0, size);
    glVertex3f(size * widthScale, 0, size);
    glVertex3f(size * widthScale, 0, 0);

    glVertex3f(0, 0, 0);

    // left
    // glColor3ub(223, 120, 239);
    glVertex3f(0, size, size);
    glVertex3f(0, 0, size);
    glVertex3f(0, 0, 0);
    glVertex3f(0, size, 0);

    // bottom
    // glColor3ub(128, 226, 126);
    glVertex3f(0, size, 0);
    glVertex3f(size * widthScale, size, 0);
    glVertex3f(size * widthScale, 0, 0);
    glVertex3f(0, 0, 0);

    // right
    // glColor3ub(255, 255, 114);
    glVertex3f(size * widthScale, 0, size);
    glVertex3f(size * widthScale, size, size);
    glVertex3f(size * widthScale, size, 0);
    glVertex3f(size * widthScale, 0, 0);

    // behind
    // glColor3ub(255, 201, 71);
    glVertex3f(size * widthScale, size, size);
    glVertex3f(0, size, size);
    glVertex3f(0, size, 0);
    glVertex3f(size * widthScale, size, 0);

    // top
    glVertex3f(0, size, size);
    glVertex3f(size * widthScale, size, size);
    glVertex3f(size * widthScale, 0, size);
    glVertex3f(0, 0, size);

    glEnd();
}

void drawPyramid()
{
    glPushMatrix();
    glTranslatef(0, -0.5f, 0);
    //glRotatef(rotateDeg, 1, 1, 1);
    glBegin(selectedGlewDrawStyles);
    {
        glTexCoord2f(0.0f, 1);
        glVertex3f(0, 1, -0.5);
        glTexCoord2f(-0.5f, 0);
        glVertex3f(-0.5, 0, 0);
        glTexCoord2f(0.5f, 0);
        glVertex3f(0.5, 0, 0);

        glTexCoord2f(0.0f, 1);
        glVertex3f(0, 1, -0.5);
        glTexCoord2f(-0.5f, 0);
        glVertex3f(-0.5, 0, 0);
        glTexCoord2f(0.5f, 0);
        glVertex3f(-0.5, 0, -1.0);

        glTexCoord2f(0.0f, 1);
        glVertex3f(0, 1, -0.5);
        glTexCoord2f(-0.5f, 0);
        glVertex3f(0.5, 0, 0);
        glTexCoord2f(0.5f, 0);
        glVertex3f(0.5, 0, -1.0);

        glTexCoord2f(0.0f, 1);
        glVertex3f(0, 1, -0.5);
        glTexCoord2f(-0.5f, 0);
        glVertex3f(0.5, 0, -1.0);
        glTexCoord2f(0.5f, 0);
        glVertex3f(-0.5, 0, -1.0);
    }
    glEnd();

    glBegin(selectedGlewDrawStyles);
    {
        glTexCoord2f(0.0f, 1);
        glVertex3f(-0.5, 0, -1);

        glTexCoord2f(1, 1);
        glVertex3f(-0.5, 0, 0);

        glTexCoord2f(1, 0.0f);
        glVertex3f(0.5, 0, 0);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.5, 0, -1);
    }
    glEnd();

    glPopMatrix();
}

void drawRightPyramid(float length)
{
    glPushMatrix();
    glTranslatef(0, -0.5f, 0);
    //glRotatef(rotateDeg, 1, 1, 1);
    glBegin(selectedGlewDrawStyles);
    {
        glTexCoord2f(0.0f, 1);
        glVertex3f(0, 1, 0);
        glTexCoord2f(-0.5f, 0);
        glVertex3f(-0.5, 0, 0);
        glTexCoord2f(0.5f, 0);
        glVertex3f(0.5, 0, 0);

        glTexCoord2f(0.0f, 1);
        glVertex3f(0, 1, 0);
        glTexCoord2f(-0.5f, 0);
        glVertex3f(-0.5, 0, 0);
        glTexCoord2f(0.5f, 0);
        glVertex3f(-0.5, 0, length);

        glTexCoord2f(0.0f, 1);
        glVertex3f(0, 1, 0);
        glTexCoord2f(-0.5f, 0);
        glVertex3f(0.5, 0, 0);
        glTexCoord2f(0.5f, 0);
        glVertex3f(0.5, 0, length);

        glTexCoord2f(0.0f, 1);
        glVertex3f(0, 1, 0);
        glTexCoord2f(-0.5f, 0);
        glVertex3f(0.5, 0, length);
        glTexCoord2f(0.5f, 0);
        glVertex3f(-0.5, 0, length);
    }
    glEnd();

    glBegin(selectedGlewDrawStyles);
    {
        glTexCoord2f(0.0f, 1);
        glVertex3f(-0.5, 0, length);

        glTexCoord2f(1, 1);
        glVertex3f(-0.5, 0, 0);

        glTexCoord2f(1, 0.0f);
        glVertex3f(0.5, 0, 0);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.5, 0, length);
    }
    glEnd();

    glPopMatrix();
}

void drawTrianglePyramid(float length)
{
    glPushMatrix();
    glTranslatef(0, -0.5f, 0);
    //glRotatef(rotateDeg, 1, 1, 1);
    glBegin(selectedGlewDrawStyles);
    {
        // left
        glTexCoord2f(0.0f, 1);
        glVertex3f(0, 1, length);
        glTexCoord2f(0.0f, 1);
        glVertex3f(0, 1, 0);
        glTexCoord2f(-0.5f, 0);
        glVertex3f(-0.5, 0, 0);
        glTexCoord2f(0.5f, 0);
        glVertex3f(0.5, 0, 0);

        // front
        glTexCoord2f(0.0f, 1);
        glVertex3f(0, 1, 0);
        glTexCoord2f(-0.5f, 0);
        glVertex3f(-0.5, 0, 0);
        glTexCoord2f(0.5f, 0);
        glVertex3f(-0.5, 0, length);

        // right
        glTexCoord2f(0.0f, 1);
        glVertex3f(0, 1, length);
        glTexCoord2f(0.0f, 1);
        glVertex3f(0, 1, 0);
        glTexCoord2f(-0.5f, 0);
        glVertex3f(0.5, 0, 0);
        glTexCoord2f(0.5f, 0);
        glVertex3f(0.5, 0, length);

        glTexCoord2f(0.0f, 1);
        glVertex3f(0, 1, length);
        glTexCoord2f(-0.5f, 0);
        glVertex3f(0.5, 0, length);
        glTexCoord2f(0.5f, 0);
        glVertex3f(-0.5, 0, length);
    }
    glEnd();

    glBegin(selectedGlewDrawStyles);
    {
        glTexCoord2f(0.0f, 1);
        glVertex3f(-0.5, 0, length);

        glTexCoord2f(1, 1);
        glVertex3f(-0.5, 0, 0);

        glTexCoord2f(1, 0.0f);
        glVertex3f(0.5, 0, 0);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.5, 0, length);
    }
    glEnd();

    glPopMatrix();
}

void drawCapsule(float width, float height)
{
    glPushMatrix();
    {
        // one cylinder and two sphere
        glPushMatrix();
        {
            // glTranslatef(0.25, 1.88 + calfHeight * 1.45, 0.04);
            glScalef(1, 1, 1);
            drawCylinder(height, height, width, 20, 10);
        }
        glPopMatrix();

        glPushMatrix();
        {
            int jointSliceStack = 10;
            float jointRadius = height;
            // glTranslatef(0.25, 1.88 + calfHeight * 1.45, 0.33);
            drawSphere(jointRadius, jointSliceStack, jointSliceStack);
            glTranslatef(0, 0, width);
            drawSphere(jointRadius, jointSliceStack, jointSliceStack);
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void drawMoon()
{
    float x = 0, y = 0, z = -15, GL_PI = 3.142, radius = 0.4;
    glPushMatrix();
    {
        // Call only once for all remaining points
        glBegin(selectedGlDrawStyles);

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

    glRotatef(-90, 0, 0, 1);
    glRotatef(-90, 1, 0, 0);

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

    glPushMatrix();
    {
        drawBracers();
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(0, -0.44, 0);
        glRotatef(0 + handDegree, 1, 0, 0);
        glTranslatef(0, 0.49, 0);
        glPushMatrix();
        {
            glRotatef(90, 0, 1, 0);
            glTranslatef(-0.04, -0.78, -0.18);
            drawCuboid(0.35, 0.21);
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(-0.1, -0.71, 0.03);
            glScalef(0.37, 0.37, 0.37);
            drawFingers();
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void drawCalf()
{
    glTranslatef(0, 2.04, 0);
    glRotatef(0 + thighDegree, 0, 0, 1);
    glTranslatef(0, -2.04, 0); //-1.84
    glPushMatrix();
    {
        glTranslatef(0.16, 0.2, 0);
        glRotatef(0 + calfDegree, 0, 0, 1);
        glTranslatef(-0.31, -2.62, 0); //-1.84
        // TODO: limit calfHeight to reasonable value
        float calfHeight = robotHeight;
        glPushMatrix();
        {
            glTranslatef(0.1, 0.45, 0);
            glScalef(0.5, 2.42 + calfHeight * 3, 0.8);
            drawCuboid(0.5, 1.51);
        }
        glPopMatrix();

        glPushMatrix();
        {
            float cylinderHeight = 1.5;
            glRotatef(180, 0, 1, 1);
            glTranslatef(-0.06, 0, 0.16);
            glScalef(1, 1, 1 + calfHeight);
            drawCylinder(0.05, 0.05, cylinderHeight, 20, 10);
            glTranslatef(0, 0.19, 0);
            drawCylinder(0.05, 0.05, cylinderHeight, 20, 10);
            glTranslatef(0, 0.19, 0);
            drawCylinder(0.05, 0.05, cylinderHeight, 20, 10);
        }
        glPopMatrix();

        ///////////////////////
        // draw calf joint
        // one cylinder and two sphere
        glPushMatrix();
        {
            float cylinderHeight = 0.31;
            glTranslatef(0.25, 1.88 + calfHeight * 1.45, 0.04);
            glScalef(1, 1, 1);
            drawCylinder(0.16, 0.16, cylinderHeight, 20, 10);
        }
        glPopMatrix();

        glPushMatrix();
        {
            int jointSliceStack = 10;
            float jointRadius = 0.16;
            glTranslatef(0.25, 1.88 + calfHeight * 1.45, 0.33);
            drawSphere(jointRadius, jointSliceStack, jointSliceStack);
            glTranslatef(0, 0, -0.27);
            drawSphere(jointRadius, jointSliceStack, jointSliceStack);
        }
        glPopMatrix();

        glPushMatrix();
        {
            // triangle cover the cylinder joint
            float pyramidSize = 0.1;
            glRotatef(270, 0, 1, 0);
            glScalef(0.39, 0.58, 0.48);
            glTranslatef(0.50, 3.4 + calfHeight * 2.5, -0.04);
            drawRightPyramid(-0.82);

            glRotatef(180, 0, 1, 0);
            glTranslatef(-0.07, 0, 0.92);
            drawRightPyramid(-0.82);
        }
        glPopMatrix();
        ///////////////////////

        // triangle shoe slope
        glPushMatrix(); // front shoe pyramid
        {
            float pyramidSize = 0.1;
            glRotatef(270, 0, 1, 0);
            glScalef(0.39, 0.58, 0.48);
            glTranslatef(0.48, 1.27, -1.0);
            drawRightPyramid(-0.67);
        }
        glPopMatrix();

        glPushMatrix();
        {
            int jointSliceStack = 10;
            float jointRadius = 0.1;
            glTranslatef(0.09, 0.210000, 0.39);
            drawSphere(jointRadius, jointSliceStack, jointSliceStack);
            glTranslatef(0, 0, -0.2);
            drawSphere(jointRadius, jointSliceStack, jointSliceStack);
            glTranslatef(0, 0, -0.2);
            drawSphere(jointRadius, jointSliceStack, jointSliceStack);
        }
        glPopMatrix();

        glPushMatrix();
        {
            glScalef(1, 0.3, 1);
            glTranslatef(0, 0, -0.05);
            drawCuboid(0.5, 2);
        }
        glPopMatrix();
        glPushMatrix();
        {
            glTranslatef(0.1, 0.3 * 0.5, 0);
            glScalef(0.7, 0.6, 0.8);
            drawCuboid(0.5, 2);
        }
        glPopMatrix();

        glPushMatrix();
        {
            float pyramidSize = 0.1;
            glRotatef(90, 0, 0, 1);
            glScalef(0.2, 0.2, 0.15);
            glTranslatef(0.52, 0.46, 0.85);
            drawPyramid();
            glTranslatef(0, 0, 1.0);
            drawPyramid();
            glTranslatef(0, 0, 1.0);
            drawPyramid();
        }
        glPopMatrix();
        glPushMatrix();
        {
            float pyramidSize = 0.1;
            glRotatef(90, 1, 0, 0);
            glScalef(0.2, 0.2, 0.15);
            glTranslatef(0.5, 2.74, 0.02);
            drawPyramid();
            glTranslatef(1, 0, 0);
            drawPyramid();
            glTranslatef(1, 0, 0);
            drawPyramid();
            glTranslatef(1, 0, 0);
            drawPyramid();
            glTranslatef(1, 0, 0);
            drawPyramid();
        }
        glPopMatrix();
        glPushMatrix();
        {
            float pyramidSize = 0.1;
            glRotatef(-90, 1, 0, 0);
            glScalef(0.2, 0.2, 0.15);
            glTranslatef(0.57, 0.65, 0.98);
            drawPyramid();
            glTranslatef(1, 0, 0);
            drawPyramid();
            glTranslatef(1, 0, 0);
            drawPyramid();
            glTranslatef(1, 0, 0);
            drawPyramid();
            glTranslatef(1, 0, 0);
            drawPyramid();
        }
        glPopMatrix();
        glPushMatrix(); // front shoe pyramid
        {
            float pyramidSize = 0.1;
            glRotatef(-90, 0, 0, 1);
            glScalef(0.15, 0.13, 0.48);
            glTranslatef(-0.49, 8.21, 0.95);
            drawPyramid();
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void drawBackground()
{
    // draw fence
    glPushMatrix();
    {
        float cylinderHeight = 0.6;
        glRotatef(-90, 1, 0, 0);
        glScalef(0.2, 0.2, 0.15);
        glTranslatef(0.57, 0.65, 0.98);
        drawPyramid();
    }
    glPopMatrix();
}

void drawThigh()
{
    glPushMatrix();
    {
        glTranslatef(0, 2.04, 0);
        glRotatef(0 + thighDegree, 0, 0, 1);
        glTranslatef(0, -2.04, 0); //-1.84

        // glRotatef(0 + thighDegree, 0, 0, 1);
        // glTranslatef(0, -1.81, 0);

        glTranslatef(-0.1, -0.43 + robotHeight * 1.5, -0.02);
        glScalef(0.5, 3.18, 0.8);
        drawCuboid(0.5, 1.51);
    }
    glPopMatrix();
}

void drawHipJoint()
{
    glPushMatrix();
    {
        glTranslatef(0, -0.17, 0.04);
        glPushMatrix();
        {
            float cylinderHeight = 1.46;
            glRotatef(90, 0, 1, 0);
            glTranslatef(0.12, 1.48 + robotHeight * 1.45, -0.47);
            glScalef(1, 1, 1);
            drawCylinder(0.22, 0.22, cylinderHeight, 20, 10);
        }
        glPopMatrix();

        glPushMatrix();
        {
            int jointSliceStack = 10;
            float jointRadius = 0.22;
            glTranslatef(0.95, 1.48 + robotHeight * 1.45, -0.12);
            drawSphere(jointRadius, jointSliceStack, jointSliceStack);
            glTranslatef(-1.37, 0, 0);
            drawSphere(jointRadius, jointSliceStack, jointSliceStack);
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void drawBody()
{
    // hip
    glPushMatrix();
    {
        glRotatef(180, 0, 0, 1);
        glTranslatef(-0.17, -1.93, 0.42);
        glScalef(1.28, 1.32, 1);
        drawTrianglePyramid(-1);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-0.59, 2.49, -0.62);
        glScalef(0.65, 2.21, 1.23);
        drawCuboid(0.88, 2.64);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-1.23, 4.44, -0.62);
        glScalef(0.65, 1.82, 1.26);
        drawCuboid(0.89, 4.83);
    }
    glPopMatrix();
}

void drawShoulderJoint()
{
    glPushMatrix();
    {
        glRotatef(90, 0, 1, 0);
        glTranslatef(0.02, 5.67, 1.32);
        drawCapsule(0.48, 0.16);
        glTranslatef(0, 0, -2.79);
        drawCapsule(0.48, 0.16);
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

    glScalef(0.7, 0.7, 0.7);

    glPushMatrix();
    {
        glPushMatrix();
        {
            glRotatef(90, 0, 1, 0);
            glTranslatef(0, 0, 0.7);
            drawThigh();

            glPushMatrix();
            {
                drawCalf();
            }
            glPopMatrix();
        }
        glPopMatrix();

        glPushMatrix();
        {
            glRotatef(90, 0, 1, 0);
            glTranslatef(0, 0, -0.7);
            drawThigh();

            glPushMatrix();
            {
                drawCalf();
            }
            glPopMatrix();
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(-0.12, -0.12, 0);
            glScalef(1.11, 1, 1);
            drawHipJoint();
        }
        glPopMatrix();
    }
    glPopMatrix();

    /////////////////////////// arm
    glPushMatrix();
    {
        float handDistance = 1.64;
        glTranslatef(0.23, 3.97, 0);
        glScalef(1.15, 1.39, 1.16);
        glPushMatrix();
        {
            // glRotatef(90, 0, 1, 0);
            glTranslatef(handDistance, 0, 0);
            drawArm();
        }
        glPopMatrix();

        glPushMatrix();
        {
            // glRotatef(90, 0, 1, 0);
            glTranslatef(-handDistance, 0, 0);
            drawArm();
        }
        glPopMatrix();
    }
    glPopMatrix();
    ///////////////////////////

    glPushMatrix();
    {
        drawBody();
    }
    glPopMatrix();

    glPushMatrix();
    {
        drawShoulderJoint();
    }
    glPopMatrix();

    // glPushMatrix();
    // {
    //     glRotatef(90, 0, 1, 0);
    //     glTranslatef(0, 0, -0.7);
    //     drawShoe();
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
