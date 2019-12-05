#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>
#include <string>
#define WINDOW_TITLE "OpenGL Window"

void drawArm();
void drawFingers();

void drawCone();
void drawTower();
void drawPencil();

void drawCloud(float size);
void drawCirle(float x1, float y1, float radius);

void drawSphere(float radius, int slice, int slack);
void drawCuboid(float x, float widthness, float thickness, float longness);
void drawCylinder(double baseRadius, double topRadius, double height, int slices, int stacks);
void drawBridge();
void drawBridgeLine();
void drawMoon();
void drawClouds();

float speed = 0;
float perspectiveX = 0, perspectiveY = 0, perspectiveZ = -5;
float orthoX = 0, orthoY = 0.5, orthoZ = 0;
float bridgeDegree = 0, bridgeLineUp = 0;

bool rotate = false;
bool isOrtho = false;
bool isLift = false;

double w = 1920;
double h = 1080;
double ar = w / h; // aspect ratio

// use dedicated GPU to run
extern "C"
{
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}
extern "C"
{
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
            PostQuitMessage(0);
        else if (wParam == VK_F1)
        {
            if (isOrtho)
                isOrtho = false;
            else
                isOrtho = true;
        }
        else if (wParam == VK_F2)
        {
            if (isLift)
                isLift = false;
            else
                isLift = true;
        }
        else if (wParam == VK_SPACE)
        {
            if (rotate)
                rotate = false;
            else
                rotate = true;
        }
        else if (wParam == VK_RIGHT)
        {
            perspectiveX -= 0.1;
            orthoX -= 0.1;
        }
        else if (wParam == VK_LEFT)
        {
            perspectiveX += 0.1;
            orthoX += 0.1;
        }
        else if (wParam == VK_UP)
        {
            perspectiveY -= 0.1;
            orthoY -= 0.1;
        }
        else if (wParam == VK_DOWN)
        {
            perspectiveY += 0.1;
            orthoY += 0.1;
        }
        else if (wParam == VK_NUMPAD8)
        {
            perspectiveZ += 0.1;
            orthoZ += 0.1;
        }
        else if (wParam == VK_NUMPAD2)
        {
            perspectiveZ -= 0.1;
            orthoZ -= 0.1;
        }
    default:
        break;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
    PIXELFORMATDESCRIPTOR pfd;
    ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

    pfd.cAlphaBits = 8;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 0;

    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

    pfd.iLayerType = PFD_MAIN_PLANE;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;

    // choose pixel format returns the number most similar pixel format available
    int n = ChoosePixelFormat(hdc, &pfd);

    // set pixel format returns whether it sucessfully set the pixel format
    if (SetPixelFormat(hdc, n, &pfd))
    {
        return true;
    }
    else
    {
        return false;
    }
}
//--------------------------------------------------------------------

void display()
{
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glRotatef(speed, 0, 1, 0);
    glRotatef(90, 0, 1, 0);

    glPushMatrix();
    {
        glRotatef(90, 0, 1, 0);
        glTranslatef(0.7, 0, 0);
        drawArm();
    }
    glPopMatrix();

    glPushMatrix();
    {
        glRotatef(90, 0, 1, 0);
        glTranslatef(-0.7, 0, 0);
        drawArm();
    }
    glPopMatrix();

    // glPushMatrix();
    // {
    //         drawBridgeCuboid(1.5, 4, 10, 1);
    // }
    // glPopMatrix();
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

// void drawCuboid(float topLeftX, float topLeftY, float topLeftZ,
//                 float topRightX, float topRightY, float topRightZ,
//                 float botLeftX, float botLeftY, float botLeftZ,
//                 float botRightX, float botRightY, float botRightZ)
// {
//     glPushMatrix();
//     glBegin(GL_QUADS);
//     //Face 1
//     glTexCoord2f(0, 1);
//     glVertex3f(topLeftX, topLeftY, topLeftZ);
//     glTexCoord2f(0, 0);
//     glVertex3f(botLeftX, botLeftY, botLeftZ);
//     glTexCoord2f(1, 0);
//     glVertex3f(botRightX, botRightY, botRightZ);
//     glTexCoord2f(1, 1);
//     glVertex3f(topRightX, topRightY, topRightZ);
//     //Face 2
//     glTexCoord2f(0, 1);
//     glVertex3f(topLeftX, topLeftY, topLeftZ);
//     glTexCoord2f(0, 0);
//     glVertex3f(botLeftX, botLeftY, botLeftZ);
//     glTexCoord2f(1, 0);
//     glVertex3f(-botLeftX, botLeftY, botLeftZ);
//     glTexCoord2f(1, 1);
//     glVertex3f(-topLeftX, topLeftY, topLeftZ);
//     //Face 3
//     glTexCoord2f(0, 1);
//     glVertex3f(-topLeftX, topLeftY, topLeftZ);
//     glTexCoord2f(0, 0);
//     glVertex3f(-botLeftX, botLeftY, botLeftZ);
//     glTexCoord2f(1, 0);
//     glVertex3f(-botRightX, botRightY, botRightZ);
//     glTexCoord2f(1, 1);
//     glVertex3f(-topRightX, topRightY, topRightZ);
//     //Face 4
//     glTexCoord2f(0, 1);
//     glVertex3f(-topRightX, topRightY, topRightZ);
//     glTexCoord2f(0, 0);
//     glVertex3f(-botRightX, botRightY, botRightZ);
//     glTexCoord2f(1, 0);
//     glVertex3f(botRightX, botRightY, botRightZ);
//     glTexCoord2f(1, 1);
//     glVertex3f(topRightX, topRightY, topRightZ);
//     //Face 5
//     glTexCoord2f(0, 1);
//     glVertex3f(topRightX, topRightY, topRightZ);
//     glTexCoord2f(0, 0);
//     glVertex3f(topLeftX, topLeftY, topLeftZ);
//     glTexCoord2f(1, 0);
//     glVertex3f(-topLeftX, topLeftY, topLeftZ);
//     glTexCoord2f(1, 1);
//     glVertex3f(-topRightX, topRightY, topRightZ);

//     glEnd();
//     glPopMatrix();
// }

void drawCuboid(float x, float widthness, float thickness, float longness)
{
    glBegin(GL_LINE_LOOP);
    // front
    glVertex3f(0, 0, x + widthness);
    glVertex3f(x + thickness, 0, x + widthness);
    glVertex3f(x + thickness, 0, 0);
    glVertex3f(0, 0, 0);

    // left
    glVertex3f(0, x + longness, x + widthness);
    glVertex3f(0, 0, x + widthness);
    glVertex3f(0, 0, 0);
    glVertex3f(0, x + longness, 0);

    // bottom
    glVertex3f(0, x + longness, 0);
    glVertex3f(x + thickness, x + longness, 0);
    glVertex3f(x + thickness, 0, 0);
    glVertex3f(0, 0, 0);

    // right
    glVertex3f(x + thickness, 0, x + widthness);
    glVertex3f(x + thickness, x + longness, x + widthness);
    glVertex3f(x + thickness, x + longness, 0);
    glVertex3f(x + thickness, 0, 0);

    // behind
    glVertex3f(x + thickness, x + longness, x + widthness);
    glVertex3f(0, x + longness, x + widthness);
    glVertex3f(0, x + longness, 0);
    glVertex3f(x + thickness, x + longness, 0);

    // top
    glVertex3f(0, x + longness, x + widthness);
    glVertex3f(x + thickness, x + longness, x + widthness);
    glVertex3f(x + thickness, 0, x + widthness);
    glVertex3f(0, 0, x + widthness);

    glEnd();
}

void drawCylinder(double baseRadius, double topRadius, double height, int slices, int stacks)
{
    GLUquadricObj *cylinder = NULL;
    cylinder = gluNewQuadric();
    gluQuadricDrawStyle(cylinder, GLU_FILL);
    gluCylinder(cylinder, baseRadius, topRadius, height, slices, stacks);
    gluDeleteQuadric(cylinder);
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

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpfnWndProc = WindowProcedure;
    wc.lpszClassName = WINDOW_TITLE;
    wc.style = CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClassEx(&wc))
        return false;

    HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT, 1920, 1080,
                             NULL, NULL, wc.hInstance, NULL);

    //--------------------------------
    //	Initialize window for OpenGL
    //--------------------------------

    HDC hdc = GetDC(hWnd);

    //	initialize pixel format for the window
    initPixelFormat(hdc);

    //	get an openGL context
    HGLRC hglrc = wglCreateContext(hdc);

    //	make context current
    if (!wglMakeCurrent(hdc, hglrc))
        return false;

    //--------------------------------
    //	End initialization
    //--------------------------------

    ShowWindow(hWnd, nCmdShow);

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
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

        SwapBuffers(hdc);
    }
    UnregisterClass(WINDOW_TITLE, wc.hInstance);
    return true;
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

    // hand
    glPushMatrix();
    {
        glRotatef(180, 0, 1, 1);
        glTranslatef(0, 0, -0.2);
        drawCylinder(0.15, 0.2, 0.2, 20, 10);
    }
    glPopMatrix();
    // hand
    glPushMatrix();
    {
        glRotatef(180, 0, 1, 1);
        glTranslatef(0, 0, -0.25);
        drawCylinder(0.15, 0.2, 0.2, 20, 10);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glRotatef(180, 0, 1, 1);
        glTranslatef(0, 0, -0.3);
        drawCylinder(0.15, 0.2, 0.2, 20, 10);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glRotatef(180, 0, 1, 1);
        glTranslatef(0, 0, -0.35);
        drawCylinder(0.15, 0.2, 0.2, 20, 10);
    }
    glPopMatrix();
    glPushMatrix();
    {
        glRotatef(180, 0, 1, 1);
        glTranslatef(0, 0, -0.4);
        drawCylinder(0.15, 0.2, 0.2, 20, 10);
    }
    glPopMatrix();
    glPushMatrix();
    {
        glRotatef(180, 0, 1, 1);
        glTranslatef(0, 0, -0.45);
        drawCylinder(0.15, 0.2, 0.2, 20, 10);
    }
    glPopMatrix();

    drawFingers();
}

void drawFingers()
{
    // longness of finger
    float indexRingFinger = 0;
    float middleFinger = 0.01;
    float littleFinger = -0.02;
    float thumb = -0.02;
    float secondY = -0.04, secondZ = 0.2;

    // first level fingers
    glPushMatrix();
    {
        glPushMatrix();
        {
            glTranslatef(0, -0.6 - (middleFinger * 1), 0);
            drawCuboid(0.04, 0, 0, 0.1 + middleFinger);
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(0.05, -0.6 - (indexRingFinger * 1), 0);
            drawCuboid(0.04, 0, 0, 0.1 + indexRingFinger);
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(-0.05, -0.6 - (indexRingFinger * 1), 0);
            drawCuboid(0.04, 0, 0, 0.1 + indexRingFinger);
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(-0.10, -0.6 - (littleFinger * 1), 0);
            drawCuboid(0.04, 0, 0, 0.1 + littleFinger);
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(0.12, -0.55 - (thumb * 1), 0);
            glRotatef(35, 0, 0, 1);
            drawCuboid(0.04, 0, 0, 0.1 + thumb);
        }
        glPopMatrix();
    }
    glPopMatrix();

    // second level fingers
    glPushMatrix();
    {
        glTranslatef(0, secondY, secondZ);
        glRotatef(20, 1, 0, 0);
        glPushMatrix();
        {
            glTranslatef(0, -0.73 - (middleFinger * 2), 0);
            drawCuboid(0.04, 0, 0, 0.08 + middleFinger);
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(0.05, -0.73 - (indexRingFinger * 2), 0);
            drawCuboid(0.04, 0, 0, 0.08 + indexRingFinger);
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(-0.05, -0.73 - (indexRingFinger * 2), 0);
            drawCuboid(0.04, 0, 0, 0.08 + indexRingFinger);
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(-0.10, -0.73 - (littleFinger * 2), 0);
            drawCuboid(0.04, 0, 0, 0.08 + littleFinger);
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(0.12, -0.7 - (thumb * 2), 0);
            glRotatef(0, 0, 0, 1);
            drawCuboid(0.04, 0, 0, 0.1 + thumb);
        }
        glPopMatrix();
    }
    glPopMatrix();

    // third level fingers
    glPushMatrix();
    {
        glTranslatef(0, secondY * 4, secondZ * 2);
        glRotatef(40, 1, 0, 0);
        glPushMatrix();
        {
            glTranslatef(0, -0.83 - (middleFinger * 3), 0);
            drawCuboid(0.04, 0, 0, 0.05 + middleFinger);
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(0.05, -0.83 - (indexRingFinger * 3), 0);
            drawCuboid(0.04, 0, 0, 0.05 + indexRingFinger);
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(-0.05, -0.83 - (indexRingFinger * 3), 0);
            drawCuboid(0.04, 0, 0, 0.05 + indexRingFinger);
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(-0.10, -0.83 - (littleFinger * 3), 0);
            drawCuboid(0.04, 0, 0, 0.05 + littleFinger);
        }
        glPopMatrix();
    }
    glPopMatrix();

    // first and second level finger joint
    glPushMatrix();
    {
        int jointSliceStack = 10;
        float jointRadius = 0.025;
        float jointX = 0.015;
        glPushMatrix();
        {
            glTranslatef(jointX, -0.6 - (middleFinger * 1), 0.01);
            drawSphere(jointRadius, jointSliceStack, jointSliceStack);
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(jointX + 0.05, -0.6 - (indexRingFinger * 1), 0.01);
            drawSphere(jointRadius, jointSliceStack, jointSliceStack);
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(jointX - 0.05, -0.6 - (indexRingFinger * 1), 0.01);
            drawSphere(jointRadius, jointSliceStack, jointSliceStack);
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(jointX - 0.10, -0.6 - (littleFinger * 1), 0.01);
            drawSphere(jointRadius, jointSliceStack, jointSliceStack);
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(jointX + 0.12, -0.55 - (thumb * 1), 0.02);
            glRotatef(35, 0, 0, 1);
            drawSphere(jointRadius, jointSliceStack, jointSliceStack);
        }
        glPopMatrix();
    }
    glPopMatrix();

    // second and third level finger joint
    glPushMatrix();
    {
        float jointX = 0.015;
        int jointSliceStack = 10;
        float jointRadius = 0.025;
        glTranslatef(0, secondY, secondZ);
        glRotatef(20, 1, 0, 0);
        glPushMatrix();
        {
            glTranslatef(jointX, -0.73 - (middleFinger * 1), 0.01);
            drawSphere(jointRadius, jointSliceStack, jointSliceStack);
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(jointX + 0.05, -0.73 - (indexRingFinger * 1), 0.01);
            drawSphere(jointRadius, jointSliceStack, jointSliceStack);
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(jointX - 0.05, -0.73 - (indexRingFinger * 1), 0.01);
            drawSphere(jointRadius, jointSliceStack, jointSliceStack);
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(jointX - 0.10, -0.73 - (littleFinger * 1), 0.01);
            drawSphere(jointRadius, jointSliceStack, jointSliceStack);
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void drawSphere(float radius, int slice, int slack)
{
    GLUquadricObj *sphere = NULL;
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_LINE);
    gluSphere(sphere, radius, slice, slack);
    gluDeleteQuadric(sphere);
}